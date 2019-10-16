REM Create a shell for running our commands
Set shell = CreateObject("WScript.Shell")

REM Get the directory containing this script file
OPENCOR_DIR = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

REM Ensure the path to Python is correct before starting Jupyter
updatePythonPath = """"&OPENCOR_DIR&"\Python\bin\Python.exe"" """&OPENCOR_DIR&"\Python\Scripts\setpythonpath.py"" """&OPENCOR_DIR&"\Python"" -s"
shell.Run updatePythonPath, 0, True

REM Start a Jupyter Lab server
cmd = """"&OPENCOR_DIR&"\Python\bin\Python.exe"" """&OPENCOR_DIR&"\Python\Scripts\start_jupyter.py"" lab --ip 127.0.0.1"
shell.Run cmd, 1, False
