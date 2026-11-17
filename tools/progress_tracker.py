#!/usr/bin/env python3
"""Project Ringsmith progress tracker: a small GUI (tkinter, no extra packages), laid out after Project Twinblade's.

Data lives in docs/progress_data.json (format in docs/tracker.md): three tracks (Decomp, Recomp, PC Port), each with
measured metrics (done out of total; a null total shows as "not measured yet", never as a percent) and phase items;
the Phase 0 milestones; and the ER-NNN task list. The program reads and writes that file; the dispatcher (and cards,
for their own items) update the same file, and the window reloads it when it changes on disk.

Tabs: Overview | one per track (Decomp, Recomp, PC Port, Assets) | Tasks (ER-NNN) | Phase 0 milestones
Click a row to edit it in the form below the list; double-click a task or milestone to step its status.
A bad status, track or missing field shows a [?] marker; it never crashes.

With arguments it updates the file without a window (for sessions): `show`, `task`, `metric`, `milestone`, `item`;
see docs/tracker.md or `python tools/progress_tracker.py -h`.
"""
import datetime
import json
import os
import tkinter as tk
from tkinter import ttk

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, 'docs', 'progress_data.json')
TRACK_IDS = ['decomp', 'recomp', 'port', 'assets', 'other']
TRACK_NAMES = {'decomp': 'Decomp', 'recomp': 'Recomp', 'port': 'PC Port', 'assets': 'Assets', 'other': 'Other'}
ITEM_STATUSES = ['Not started', 'In progress', 'Complete']
TASK_STATUSES = ['Queued', 'In progress', 'Waiting', 'Done']
COLORS = {'Complete': '#1b7f3b', 'Done': '#1b7f3b', 'In progress': '#b36b00', 'Not started': '#777777',
          'Queued': '#555555', 'Waiting': '#8a6d00', 'bad': '#c0261d'}
MARKS = {'Complete': '☑', 'Done': '☑', 'In progress': '◐', 'Not started': '☐', 'Queued': '☐', 'Waiting': '⧗'}


def load():
    with open(DATA, encoding='utf-8') as f:
        return json.load(f)


def save(d):
    d['updated'] = datetime.date.today().isoformat()
    tmp = DATA + '.tmp'
    with open(tmp, 'w', encoding='utf-8', newline='\n') as f:
        json.dump(d, f, indent=2, ensure_ascii=False)
        f.write('\n')
    os.replace(tmp, DATA)


def num(v):
    """A non-negative int, or None for null / missing / not a number."""
    try:
        return None if v is None or v == '' else max(0, int(v))
    except (TypeError, ValueError):
        return None


def metric_text(m):
    """('12 / 340 functions', 3.5) or ('0 functions, total not measured yet', None)."""
    done, total, unit = num(m.get('done')) or 0, num(m.get('total')), m.get('unit', '')
    if not total:
        return f'{done:,} {unit}, total not measured yet', None
    return f'{done:,} / {total:,} {unit}', 100.0 * min(done, total) / total


def track_tag(x):
    return x.get('track') if x.get('track') in TRACK_IDS else None


def status_ok(x, allowed):
    return x.get('status') if x.get('status') in allowed else None


def task_sort_key(t):
    code = str(t.get('code', ''))
    digits = ''.join(c for c in code if c.isdigit())
    return int(digits) if digits else 10 ** 6


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('Project Ringsmith: progress')
        self.geometry('1180x720')
        self.minsize(900, 560)
        st = ttk.Style(self)
        try:
            st.theme_use('clam')
        except tk.TclError:
            pass
        st.configure('Treeview', rowheight=24)
        st.configure('Big.TLabel', font=('Segoe UI', 14, 'bold'))
        self.data, self.mtime, self.error = {}, None, ''
        self.top = ttk.Label(self, style='Big.TLabel')
        self.top.pack(anchor='w', padx=12, pady=(10, 2))
        self.nb = ttk.Notebook(self)
        self.nb.pack(fill='both', expand=True, padx=8, pady=8)
        self.reload(force=True)
        self.build()
        self.after(2000, self.poll)

    # ---- data ----
    def reload(self, force=False):
        try:
            m = os.path.getmtime(DATA)
            if force or m != self.mtime:
                self.data, self.mtime, self.error = load(), m, ''
                return True
        except (OSError, ValueError) as e:
            self.error = f'cannot read docs/progress_data.json: {e}'
            return force
        return False

    def commit(self):
        save(self.data)
        self.mtime = os.path.getmtime(DATA)
        self.build()

    def poll(self):
        if self.reload():
            self.build()
        self.after(2000, self.poll)

    def tracks(self):
        return [t for t in self.data.get('tracks', []) if isinstance(t, dict)]

    def tasks(self):
        return sorted([t for t in self.data.get('tasks', []) if isinstance(t, dict)], key=task_sort_key)

    def milestones(self):
        return [m for m in self.data.get('milestones', []) if isinstance(m, dict)]

    # ---- building ----
    def build(self):
        sel = self.nb.index('current') if self.nb.tabs() else 0
        for t in self.nb.tabs():
            self.nb.nametowidget(t).destroy()
        self.fill_overview(self.frame('Overview'))
        for tr in self.tracks():
            self.fill_track(self.frame(tr.get('name', tr.get('id', '[?]'))), tr)
        self.fill_tasks(self.frame('Tasks (ER-NNN)'))
        self.fill_milestones(self.frame('Phase 0 milestones'))
        tasks = self.tasks()
        n = {s: sum(1 for t in tasks if t.get('status') == s) for s in TASK_STATUSES}
        ms = self.milestones()
        msd = sum(1 for m in ms if m.get('status') == 'Complete')
        txt = (f'Project Ringsmith   |   tasks: {n["Done"]} done, {n["In progress"]} in progress, {n["Queued"]} queued, '
               f'{n["Waiting"]} waiting   |   Phase 0: {msd}/{len(ms)} milestones   |   data updated {self.data.get("updated", "?")}')
        self.top.config(text=self.error or txt, foreground=COLORS['bad'] if self.error else '')
        try:
            self.nb.select(sel)
        except tk.TclError:
            pass

    def frame(self, name):
        f = ttk.Frame(self.nb)
        self.nb.add(f, text=name)
        return f

    def tree(self, parent, cols, height=None):
        """A Treeview with a scrollbar, packed to fill; cols = [(id, heading, width, anchor)], the first is the tree column."""
        box = ttk.Frame(parent)
        box.pack(fill='both', expand=True, padx=8, pady=(0, 4))
        tv = ttk.Treeview(box, columns=[c[0] for c in cols[1:]], show='tree headings', selectmode='browse', height=height or 10)
        for i, (cid, head, width, anchor) in enumerate(cols):
            key = '#0' if i == 0 else cid
            tv.heading(key, text=head)
            tv.column(key, width=width, anchor=anchor, stretch=(i == 0 or cid == 'notes'))
        for s, c in COLORS.items():
            tv.tag_configure(s, foreground=c)
        tv.tag_configure('grp', font=('Segoe UI', 10, 'bold'))
        sb = ttk.Scrollbar(box, orient='vertical', command=tv.yview)
        tv.configure(yscrollcommand=sb.set)
        tv.pack(side='left', fill='both', expand=True)
        sb.pack(side='right', fill='y')
        return tv

    # ---- overview ----
    def fill_overview(self, f):
        g = ttk.Frame(f)
        g.pack(fill='both', expand=True, padx=14, pady=10)
        row = 0

        def line(label, text, pct, bold=False):
            nonlocal row
            font = ('Segoe UI', 11, 'bold' if bold else 'normal')
            ttk.Label(g, text=label, font=font, width=46).grid(row=row, column=0, sticky='w', pady=3)
            ttk.Progressbar(g, length=340, maximum=100, value=pct or 0).grid(row=row, column=1, padx=8)
            ttk.Label(g, text=f'{pct:.1f}%' if pct is not None else '--', width=7, font=font).grid(row=row, column=2)
            ttk.Label(g, text=text, foreground='#555').grid(row=row, column=3, sticky='w', padx=8)
            row += 1

        for tr in self.tracks():
            ttk.Separator(g).grid(row=row, column=0, columnspan=4, sticky='ew', pady=6)
            row += 1
            its = [i for i in tr.get('items', []) if isinstance(i, dict)]
            c = {s: sum(1 for i in its if i.get('status') == s) for s in ITEM_STATUSES}
            ttk.Label(g, text=tr.get('name', '[?]'), font=('Segoe UI', 12, 'bold')).grid(row=row, column=0, sticky='w')
            ttk.Label(g, text=f'phases: {c["Complete"]} complete, {c["In progress"]} in progress, {c["Not started"]} not started',
                      foreground='#555').grid(row=row, column=3, sticky='w', padx=8)
            row += 1
            for m in tr.get('metrics', []):
                if isinstance(m, dict):
                    text, pct = metric_text(m)
                    line('    ' + str(m.get('name', '[?]')), text, pct)
        ttk.Separator(g).grid(row=row, column=0, columnspan=4, sticky='ew', pady=6)
        row += 1
        ms = self.milestones()
        msd = sum(1 for m in ms if m.get('status') == 'Complete')
        line('Phase 0 milestones', f'{msd} of {len(ms)} complete', 100.0 * msd / len(ms) if ms else None, True)
        tasks = self.tasks()
        td = sum(1 for t in tasks if t.get('status') == 'Done')
        ttk.Label(g, text=f'ER tasks: {td} of {len(tasks)} done (task count, not a measure of progress).', foreground='#555'
                  ).grid(row=row, column=0, columnspan=4, sticky='w', pady=3)
        row += 1
        ttk.Label(g, text='\nA metric shows a percent only once its total is measured (split ER-003, census ER-004); '
                          '"--" means not measured yet. Nothing here is an estimate.',
                  foreground='#555').grid(row=row, column=0, columnspan=4, sticky='w')

    # ---- one track tab ----
    def fill_track(self, f, tr):
        tid = tr.get('id')
        ttk.Label(f, text=tr.get('summary', ''), wraplength=1100, justify='left', foreground='#444').pack(anchor='w', padx=8, pady=(8, 4))
        tv = self.tree(f, [('name', 'Metric / phase item / task', 520, 'w'), ('value', 'Value / status', 260, 'w'),
                           ('notes', 'Where it stands', 360, 'w')])
        ref = {}
        gm = tv.insert('', 'end', text='Measured progress', open=True, tags=('grp',))
        phases = {}
        for m in tr.get('metrics', []):
            if not isinstance(m, dict):
                continue
            ph = m.get('phase', '')
            if ph not in phases:
                phases[ph] = tv.insert(gm, 'end', text=ph or '[?] no phase', open=True)
            text, pct = metric_text(m)
            val = text + (f'  ({pct:.1f}%)' if pct is not None else '')
            note = m.get('notes', '') + (f'   [source: {m["source"]}]' if m.get('source') else '')
            ref[tv.insert(phases[ph], 'end', text=str(m.get('name', '[?]')), values=(val, note))] = ('metric', m)
        gi = tv.insert('', 'end', text='Phases', open=True, tags=('grp',))
        for it in tr.get('items', []):
            if not isinstance(it, dict):
                continue
            s = status_ok(it, ITEM_STATUSES)
            label = f'{it.get("phase", "")}: {it.get("name", "[?]")}'
            ref[tv.insert(gi, 'end', text=f'{MARKS.get(s, "[?]")} {label}', values=(s or f'[?] {it.get("status")}', it.get('notes', '')),
                          tags=(s or 'bad',))] = ('item', it)
        gt = tv.insert('', 'end', text='ER tasks on this track', open=True, tags=('grp',))
        for t in self.tasks():
            if t.get('track') == tid:
                s = status_ok(t, TASK_STATUSES)
                ref[tv.insert(gt, 'end', text=f'{MARKS.get(s, "[?]")} {t.get("code", "?")}: {t.get("title", "")}',
                              values=(s or f'[?] {t.get("status")}', t.get('model', '')), tags=(s or 'bad',))] = ('task', t)
        ms = [m for m in self.milestones() if m.get('track') == tid]
        if ms:
            g0 = tv.insert('', 'end', text='Phase 0 milestones on this track', open=True, tags=('grp',))
            for m in ms:
                s = status_ok(m, ITEM_STATUSES)
                ref[tv.insert(g0, 'end', text=f'{MARKS.get(s, "[?]")} {m.get("text", "[?]")}', values=(s or f'[?] {m.get("status")}', m.get('by', '')),
                              tags=(s or 'bad',))] = ('milestone', m)
        self.editor(f, tv, ref)

    # ---- tasks ----
    def fill_tasks(self, f):
        ttk.Label(f, text='ER-NNN cards (from docs/handoff.md, kept by the dispatcher). Double-click to step the status.',
                  foreground='#444').pack(anchor='w', padx=8, pady=6)
        tv = self.tree(f, [('code', 'Task', 440, 'w'), ('status', 'Status', 90, 'center'), ('track', 'Track', 80, 'center'),
                           ('model', 'Model / effort', 110, 'w'), ('waits', 'Waits on', 150, 'w'), ('notes', 'Notes', 300, 'w')])
        ref = {}
        for t in self.tasks():
            s = status_ok(t, TASK_STATUSES)
            tk_ = track_tag(t)
            ref[tv.insert('', 'end', text=f'{MARKS.get(s, "[?]")} {t.get("code", "?")}: {t.get("title", "")}',
                          values=(s or f'[?] {t.get("status")}', TRACK_NAMES[tk_] if tk_ else f'[?] {t.get("track")}',
                                  t.get('model', ''), t.get('waits_on', ''), t.get('notes', '')),
                          tags=(s or 'bad',))] = ('task', t)
        self.editor(f, tv, ref)

    # ---- milestones ----
    def fill_milestones(self, f):
        ttk.Label(f, text='Phase 0 milestones from docs/roadmap.md. Double-click to step the status.',
                  foreground='#444').pack(anchor='w', padx=8, pady=6)
        tv = self.tree(f, [('text', 'Milestone', 440, 'w'), ('status', 'Status', 90, 'center'), ('track', 'Track', 80, 'center'),
                           ('by', 'Cards', 120, 'w'), ('notes', 'Notes', 400, 'w')])
        ref = {}
        for m in self.milestones():
            s = status_ok(m, ITEM_STATUSES)
            tk_ = track_tag(m)
            ref[tv.insert('', 'end', text=f'{MARKS.get(s, "[?]")} {m.get("text", "[?]")}',
                          values=(s or f'[?] {m.get("status")}', TRACK_NAMES[tk_] if tk_ else f'[?] {m.get("track")}',
                                  m.get('by', ''), m.get('notes', '')), tags=(s or 'bad',))] = ('milestone', m)
        self.editor(f, tv, ref)

    # ---- the edit form under each list ----
    def editor(self, f, tv, ref):
        form = ttk.Frame(f)
        form.pack(fill='x', padx=8, pady=8)
        v = {k: tk.StringVar() for k in ('status', 'done', 'total', 'notes')}
        cur = {'kind': None, 'obj': None}
        lab_s = ttk.Label(form, text='Status')
        cb = ttk.Combobox(form, textvariable=v['status'], state='readonly', width=12)
        lab_d = ttk.Label(form, text='Done')
        en_d = ttk.Entry(form, textvariable=v['done'], width=9)
        lab_t = ttk.Label(form, text='Total (empty = not measured)')
        en_t = ttk.Entry(form, textvariable=v['total'], width=9)
        ttk.Label(form, text='Notes').grid(row=0, column=6)
        ttk.Entry(form, textvariable=v['notes'], width=60).grid(row=0, column=7, padx=4, sticky='ew')
        form.columnconfigure(7, weight=1)
        msg = ttk.Label(form, foreground=COLORS['bad'])
        msg.grid(row=1, column=0, columnspan=9, sticky='w')

        def show(kind):
            for w in (lab_s, cb, lab_d, en_d, lab_t, en_t):
                w.grid_remove()
            if kind == 'metric':
                lab_d.grid(row=0, column=0)
                en_d.grid(row=0, column=1, padx=4)
                lab_t.grid(row=0, column=2)
                en_t.grid(row=0, column=3, padx=4)
            elif kind:
                lab_s.grid(row=0, column=0)
                cb.grid(row=0, column=1, padx=4)
                cb.config(values=TASK_STATUSES if kind == 'task' else ITEM_STATUSES)

        def find(kind, obj):
            """The same object in freshly reloaded data (matched by id / code)."""
            key = 'code' if kind == 'task' else 'id'
            pools = {'task': self.data.get('tasks', []), 'milestone': self.data.get('milestones', []),
                     'metric': [m for t in self.tracks() for m in t.get('metrics', [])],
                     'item': [i for t in self.tracks() for i in t.get('items', [])]}[kind]
            return next((x for x in pools if isinstance(x, dict) and x.get(key) == obj.get(key)), None)

        def on_select(_e=None):
            s = tv.selection()
            kind, obj = ref.get(s[0], (None, None)) if s else (None, None)
            cur.update(kind=kind, obj=obj)
            msg.config(text='')
            show(kind)
            if obj is not None:
                v['status'].set(obj.get('status', ''))
                v['done'].set(str(obj.get('done', 0)))
                v['total'].set('' if obj.get('total') is None else str(obj.get('total')))
                v['notes'].set(obj.get('notes', ''))

        def apply(step=False):
            if cur['obj'] is None:
                return
            self.reload()
            obj = find(cur['kind'], cur['obj'])
            if obj is None:
                msg.config(text='That entry is no longer in the file (it changed on disk); pick it again.')
                return
            if cur['kind'] == 'metric':
                d, t = num(v['done'].get()), v['total'].get().strip()
                if d is None or (t and num(t) is None):
                    msg.config(text='Done and total must be whole numbers (total may be empty).')
                    return
                obj.update(done=d, total=num(t) if t else None)
            else:
                allowed = TASK_STATUSES if cur['kind'] == 'task' else ITEM_STATUSES
                if step:
                    s = obj.get('status')
                    obj['status'] = allowed[(allowed.index(s) + 1) % len(allowed)] if s in allowed else allowed[0]
                elif v['status'].get() in allowed:
                    obj['status'] = v['status'].get()
                if cur['kind'] == 'item':
                    obj['percent'] = 100 if obj['status'] == 'Complete' else (0 if obj['status'] == 'Not started' else obj.get('percent', 0))
            if not step:
                obj['notes'] = v['notes'].get()
            self.commit()

        def on_double(_e=None):
            if cur['kind'] in ('task', 'milestone', 'item'):
                apply(step=True)

        tv.bind('<<TreeviewSelect>>', on_select)
        tv.bind('<Double-1>', on_double)
        ttk.Button(form, text='Save', command=apply).grid(row=0, column=8, padx=4)


def cli(argv):
    """Command-line updates for sessions (no window). See docs/tracker.md. Returns the exit code."""
    import argparse
    ap = argparse.ArgumentParser(prog='progress_tracker.py', description='Update docs/progress_data.json without opening the window.')
    sub = ap.add_subparsers(dest='cmd', required=True)
    sub.add_parser('show', help='print every task, milestone and metric')
    t = sub.add_parser('task', help='update (or add) an ER task')
    t.add_argument('code')
    t.add_argument('--status', choices=TASK_STATUSES)
    for k in ('title', 'track', 'model', 'waits-on', 'notes'):
        t.add_argument('--' + k)
    m = sub.add_parser('metric', help='set a metric (done / total)')
    m.add_argument('id')
    m.add_argument('--done', type=int)
    m.add_argument('--total', help='a whole number, or "none" for not measured yet')
    m.add_argument('--notes')
    for kind in ('milestone', 'item'):
        p = sub.add_parser(kind, help=f'set a {kind} status (item = a phase item of a track)')
        p.add_argument('id')
        p.add_argument('--status', choices=ITEM_STATUSES)
        p.add_argument('--notes')
    a = ap.parse_args(argv)
    d = load()
    trs = [x for x in d.get('tracks', []) if isinstance(x, dict)]
    if a.cmd == 'show':
        for x in sorted(d.get('tasks', []), key=task_sort_key):
            print(f'{x.get("code")}  {x.get("status"):<11} {x.get("track"):<7} {x.get("title")}')
        for x in d.get('milestones', []):
            print(f'{x.get("id"):<6}  {x.get("status"):<11} {x.get("text")}')
        for tr in trs:
            for x in tr.get('metrics', []):
                print(f'{x.get("id"):<14} {tr.get("id"):<7} {metric_text(x)[0]}  ({x.get("name")})')
            for x in tr.get('items', []):
                print(f'{x.get("id"):<14} {tr.get("id"):<7} {x.get("status")}  ({x.get("name")})')
        return 0
    if a.cmd == 'task':
        if a.track and a.track not in TRACK_IDS:
            ap.error(f'--track must be one of {TRACK_IDS}')
        obj = next((x for x in d.setdefault('tasks', []) if x.get('code') == a.code), None)
        if obj is None:
            if not (a.title and a.track):
                ap.error(f'{a.code} is not in the file; a new task needs --title and --track')
            obj = dict(code=a.code, title='', status='Queued', track='other', model='', waits_on='', notes='')
            d['tasks'].append(obj)
        for k, v in (('status', a.status), ('title', a.title), ('track', a.track), ('model', a.model),
                     ('waits_on', a.waits_on), ('notes', a.notes)):
            if v is not None:
                obj[k] = v
    else:
        pool = {'metric': [x for tr in trs for x in tr.get('metrics', [])], 'item': [x for tr in trs for x in tr.get('items', [])],
                'milestone': d.get('milestones', [])}[a.cmd]
        obj = next((x for x in pool if isinstance(x, dict) and x.get('id') == a.id), None)
        if obj is None:
            ap.error(f'no {a.cmd} with id {a.id} (run "show" for the ids)')
        if a.cmd == 'metric':
            if a.done is not None:
                obj['done'] = max(0, a.done)
            if a.total is not None:
                obj['total'] = None if a.total.lower() == 'none' else num(a.total)
                if a.total.lower() != 'none' and obj['total'] is None:
                    ap.error('--total must be a whole number or "none"')
        else:
            if a.status:
                obj['status'] = a.status
                if a.cmd == 'item':
                    obj['percent'] = 100 if a.status == 'Complete' else (0 if a.status == 'Not started' else obj.get('percent', 0))
        if a.notes is not None:
            obj['notes'] = a.notes
    save(d)
    print('saved:', json.dumps(obj, ensure_ascii=False))
    return 0


if __name__ == '__main__':
    import sys
    if len(sys.argv) > 1:
        sys.exit(cli(sys.argv[1:]))
    App().mainloop()
