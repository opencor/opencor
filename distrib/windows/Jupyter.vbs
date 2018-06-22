REM #### Work-in-progress as Jupyter doesn't start...

REM #### Should we only start Jupyter using the Jupyter.bat??
REM #### and/or run Jupyter notebook as a Windows NT service??

REM Create a shell for running our commands
Set shell = CreateObject("WScript.Shell")

REM Get the directory containing this script file
OPENCOR_DIR = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

REM Ensure the path to Python is correct before starting Jupyter
updatePythonPath = """"&OPENCOR_DIR&"\Python\bin\Python.exe"" """&OPENCOR_DIR&"\Python\Scripts\set_python_path.py"" """&OPENCOR_DIR&"\Python"" -s"

shell.Run updatePythonPath, 0, True

REM Start Jupyter
args = ""
For Each arg in WScript.Arguments
    args = args & " " & arg
Next

Jupyter = """"&OPENCOR_DIR&"\Python\bin\Python.exe"" """&OPENCOR_DIR&"\Python\Scripts\start_jupyter.py"" " & args

Set JupyterRun = shell.Exec(Jupyter)
Do Until JupyterRun.StdOut.AtEndOfStream
    Wscript.echo objExecObject.StdOut.ReadLine()
Loop
