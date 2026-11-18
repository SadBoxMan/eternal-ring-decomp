# ER-008: turn the ps2_analyzer TOML into the two PS2Recomp configs used by the spike:
#   cfg_heur.toml  - analyzer's own JAL-scan function boundaries
#   cfg_split.toml - our split's 3,606 functions as the "Ghidra map" CSV (tools/recomp/syms_to_csv.py)
# Inputs and outputs live outside the repo (they name game addresses and point at game files).
# Usage: python -I make_configs.py <analyzer.toml> <work_dir> <split_functions.csv>
import pathlib, sys
src, work, csv = sys.argv[1], pathlib.Path(sys.argv[2]), sys.argv[3]
t = open(src, encoding='utf-8').read()
out_line = next(l for l in t.splitlines() if l.startswith('output = '))
w = work.as_posix().rstrip('/')
t = t.replace(out_line, f'output = "{w}/out_heur/"')
t = t.replace('single_file_output = false', 'single_file_output = false\noutput_worker_threads = 4', 1)
(work / 'cfg_heur.toml').write_text(t, encoding='utf-8', newline='\n')
t2 = t.replace('out_heur/', 'out_split/').replace('ghidra_output = ""', f'ghidra_output = "{pathlib.Path(csv).as_posix()}"')
(work / 'cfg_split.toml').write_text(t2, encoding='utf-8', newline='\n')
print('wrote', work / 'cfg_heur.toml', 'and', work / 'cfg_split.toml')
