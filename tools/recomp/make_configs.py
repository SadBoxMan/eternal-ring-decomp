# ER-008: turn the ps2_analyzer TOML into the two PS2Recomp configs used by the spike:
#   cfg_heur.toml  - analyzer's own JAL-scan function boundaries
#   cfg_split.toml - our split's 3,606 functions as the "Ghidra map" CSV (tools/recomp/syms_to_csv.py)
# Inputs and outputs live outside the repo (they name game addresses and point at game files).
# Usage: python -I make_configs.py <analyzer.toml> <work_dir> <split_functions.csv>
import pathlib, sys

# ER-043: libmpeg entry points the analyzer's symbol database does not name, bound here to the runtime's
# existing HLE handlers (names are our reading of the code, Unconfirmed; docs/recomp_spike.md section 10).
# Left as recompiled guest code they read libmpeg's private state (*(mp+0x40)), which the HLE MPEG path
# never writes, so the movie player never sees the end of the stream.
EXTRA_STUBS = [
    'sceMpegIsEnd@0x001DBC30',           # lw v1,0x40(a0); lw v0,0(v1): returns sys[0]
    'sceMpegIsRefBuffEmpty@0x001DBC40',  # lw v1,0x40(a0); lw v0,4(v1); sltiu v0,v0,1: returns sys[1] == 0
]

# ER-074: analyzer stubs left out, so the shipped code is recompiled instead (as shipped first). The runtime's
# HLE sceVu0Normalize takes a 4-component length and scales w too (and returns 0 below 1e-6); the shipped
# 0x001D73D0 normalises xyz by the xyz length and writes w = 0 (lqc2, vmul.xyz, vaddy.x, vaddz.x, vsqrt, vdiv,
# vsub vf6=0, vmulq.xyz, sqc2; our reading of the encodings, docs/recomp_spike.md section 13). The game calls it
# from 20+ sites. Until ER-074 it had no function boundary at all (a data-labelled block, syms_to_csv.py), so
# this binding was never used.
DROP_STUBS = [
    'sceVu0Normalize@0x001D73D0',
]

src, work, csv = sys.argv[1], pathlib.Path(sys.argv[2]), sys.argv[3]
t = open(src, encoding='utf-8').read()
out_line = next(l for l in t.splitlines() if l.startswith('output = '))
w = work.as_posix().rstrip('/')
t = t.replace(out_line, f'output = "{w}/out_heur/"')
t = t.replace('single_file_output = false', 'single_file_output = false\noutput_worker_threads = 4', 1)
missing = [s for s in EXTRA_STUBS if f'"{s}"' not in t]
if missing:
    assert t.count('stubs = [\n') == 1, 'stubs list not found'
    t = t.replace('stubs = [\n', 'stubs = [\n' + ''.join(f'  "{s}",\n' for s in missing), 1)
for s in DROP_STUBS:
    line = f'  "{s}",\n'
    if line in t:
        assert t.count(line) == 1, f'{s} listed twice'
        t = t.replace(line, '', 1)
(work / 'cfg_heur.toml').write_text(t, encoding='utf-8', newline='\n')
t2 = t.replace('out_heur/', 'out_split/').replace('ghidra_output = ""', f'ghidra_output = "{pathlib.Path(csv).as_posix()}"')
(work / 'cfg_split.toml').write_text(t2, encoding='utf-8', newline='\n')
print('wrote', work / 'cfg_heur.toml', 'and', work / 'cfg_split.toml', '; extra stubs added:', len(missing))
