#!/usr/bin/env python3
"""Backup of our own work to the external drive (zip + git bundle). Run by the dispatcher only (docs/build.md).

Writes E:\\Claude\\Project Ringsmith\\backups\\ringsmith_<YYYYMMDD>_<HHMM>_<tag>.zip (tag `main` for the main checkout,
else the worktree's folder name) with src, include, tools, docs, Programs, the yaml, symbol_addrs.txt, reloc_addrs.txt,
Makefile, requirements.txt, CLAUDE.md, .gitignore and .gitattributes, plus repo.bundle in the same folder (all refs,
rewritten each time: `git clone repo.bundle` restores every commit). Keeps the last 48 zips per tag.

Never game data or build output: the file list is what git sees (tracked files plus untracked files not ignored),
filtered to the lists below; every path is then checked against .gitignore (`git check-ignore --no-index`) and a
hard-coded deny list, and the run refuses (exit 2, nothing written) if any selected path is ignored or denied.

The bundle holds every branch of the shared repository whichever checkout writes it; while another backup is writing it
(repo.bundle.lock) this one waits up to 2 minutes, then leaves the bundle to that one. Never delete the lock by hand.

Usage: backup.py [dest dir]
"""
import datetime
import glob
import os
import re
import subprocess
import sys
import time
import zipfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DEST = sys.argv[1] if len(sys.argv) > 1 else r'E:\Claude\Project Ringsmith\backups'
DIRS = ('src/', 'include/', 'tools/', 'docs/', 'Programs/')
FILES = ('slus_200.15.yaml', 'symbol_addrs.txt', 'reloc_addrs.txt', 'Makefile', 'requirements.txt', 'CLAUDE.md',
         '.gitignore', '.gitattributes')
SKIP = ('tools/compilers/', 'tools/bin/')          # downloaded binaries (also gitignored), re-fetchable
DENY_PREFIX = ('disc/', 'asm/', 'assets/', 'baserom/', 'linker/', 'build/', 'build_logs/', 'captures/', 'screens/',
               '.venv/')
DENY_SUFFIX = ('.iso', '.elf', '.irx', '.bin', '.cue', '.o', '.pyc')
KEEP = 48


def git(*args, input=None):
    return subprocess.run(['git', *args], cwd=ROOT, capture_output=True, input=input)


def checkout_tag():
    """`main` for the main checkout, else the worktree's folder name"""
    r = git('rev-parse', '--git-dir', '--git-common-dir')
    parts = r.stdout.decode().split()[:2]
    if r.returncode or len(parts) < 2:
        return 'main'
    git_dir, common = (os.path.abspath(os.path.join(ROOT, p)) for p in parts)
    if os.path.normcase(git_dir) == os.path.normcase(common):
        return 'main'
    return re.sub(r'[^A-Za-z0-9_-]', '_', os.path.basename(ROOT)) or 'worktree'


def select_files():
    """sorted list of repo-relative paths to back up; exits 2 if any would be ignored or denied"""
    r = git('ls-files', '-z', '--cached', '--others', '--exclude-standard')
    if r.returncode:
        sys.exit('git ls-files failed: ' + r.stderr.decode().strip())
    chosen = []
    for p in sorted(set(r.stdout.decode('utf-8').split('\0')) - {''}):
        if not (p in FILES or p.startswith(DIRS)) or p.startswith(SKIP):
            continue
        if '__pycache__/' in p or not os.path.isfile(os.path.join(ROOT, p)):   # deleted but still in the index
            continue
        chosen.append(p)
    bad = [p for p in chosen if p.startswith(DENY_PREFIX) or p.lower().endswith(DENY_SUFFIX)]
    if chosen:
        c = git('check-ignore', '--no-index', '-z', '--stdin', input='\0'.join(chosen).encode('utf-8'))
        bad += [p for p in c.stdout.decode('utf-8').split('\0') if p]
    if bad:
        print('backup REFUSED: these selected paths are game data, build output or git-ignored: '
              + ', '.join(sorted(set(bad))[:20]), file=sys.stderr)
        sys.exit(2)
    return chosen


def write_bundle():
    """(ok, note): waits up to 2 minutes while another backup holds repo.bundle.lock, then gives way to it"""
    bundle = os.path.join(DEST, 'repo.bundle')
    lock = bundle + '.lock'
    deadline = time.time() + 120
    while True:
        try:
            fd = os.open(lock, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
        except FileExistsError:
            fd = None
        if fd is not None:
            try:
                os.write(fd, f'{os.getpid()}\n'.encode())
                os.close(fd)
                tmp = bundle + '.tmp'
                r = git('bundle', 'create', tmp, '--all')
                if r.returncode:
                    return False, '; bundle FAILED: ' + r.stderr.decode().strip()[:200]
                v = subprocess.run(['git', 'bundle', 'verify', tmp], cwd=ROOT, capture_output=True, text=True)
                if v.returncode:
                    return False, '; bundle verify FAILED: ' + (v.stderr or v.stdout).strip()[:200]
                os.replace(tmp, bundle)
                return True, '; bundle verify okay'
            finally:
                try:
                    os.remove(lock)
                except OSError:
                    pass
        if time.time() > deadline:
            return False, '; bundle left to the other backup that was writing it'
        time.sleep(5)


def main():
    if not os.path.isdir(os.path.dirname(DEST)):
        print(f'backup skipped: {os.path.dirname(DEST)} is not available (drive not connected?)')
        return 1
    files = select_files()
    os.makedirs(DEST, exist_ok=True)
    tag = checkout_tag()
    out = os.path.join(DEST, f'ringsmith_{datetime.datetime.now().strftime("%Y%m%d_%H%M")}_{tag}.zip')
    try:
        with zipfile.ZipFile(out, 'w', zipfile.ZIP_DEFLATED) as z:
            for p in files:
                z.write(os.path.join(ROOT, p), p)
    except BaseException:
        if os.path.exists(out):
            os.remove(out)
        raise
    ok, note = write_bundle()
    mine = [p for p in glob.glob(os.path.join(DEST, 'ringsmith_*.zip'))
            if (re.match(r'ringsmith_\d{8}_\d{4}_(.+)\.zip$', os.path.basename(p)) or [None, None])[1] == tag]
    for p in sorted(mine, key=lambda p: os.path.basename(p))[:-KEEP]:
        os.remove(p)
    head = git('rev-parse', '--short', 'HEAD').stdout.decode().strip()
    print(f'backup {out}: {len(files)} files, {os.path.getsize(out) // 1024} KB'
          + (f'; repo.bundle at {head}' if ok else '') + note)
    return 0 if ok else 1


if __name__ == '__main__':
    sys.exit(main())
