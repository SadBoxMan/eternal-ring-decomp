@echo off
rem Opens the Ringsmith progress tracker without a console window (pythonw).
cd /d "%~dp0..\.."
if exist ".venv\Scripts\pythonw.exe" (
  start "" ".venv\Scripts\pythonw.exe" tools\progress_tracker.py
) else (
  start "" pyw -3 tools\progress_tracker.py
)
