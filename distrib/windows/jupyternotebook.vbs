REM Create a shell for running our commands
Set shell = CreateObject("WScript.Shell")

REM Get the directory containing this script file
appDir = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

REM Ensure the path to Python is correct before starting Jupyter
updatePythonPath = """"&appDir&"\Python\bin\Python.exe"" """&appDir&"\Python\Scripts\setpythonpath.py"" """&appDir&"\Python"" -s"
shell.Run updatePythonPath, 0, True

REM Start a Jupyter notebook server
cmd = """"&appDir&"\Python\bin\Python.exe"" """&appDir&"\Python\Scripts\start_jupyter.py"" notebook --ip 127.0.0.1"
shell.Run cmd, 1, False
