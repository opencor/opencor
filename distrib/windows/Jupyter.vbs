REM #### Work-in-progress as Jupyter doesn't start...

REM #### Should we only start Jupyter using the Jupyter.bat??
REM #### and/or run Jupyter notebook as a Windows NT service??

REM Create a shell for running our commands
Set shell = CreateObject("WScript.Shell")

REM Get the directory containing this script file
OPENCOR_ROOT = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

REM Ensure the path to Python is correct before starting Jupyter
updatePythonPath = """"&OPENCOR_ROOT&"\Python\bin\Python.exe"" """&OPENCOR_ROOT&"\Python\Scripts\set_python_path.py"" """&OPENCOR_ROOT&"\Python"" -s"

shell.Run updatePythonPath, 0, True

REM Start Jupyter
args = ""
For Each arg in WScript.Arguments
    args = args & " " & arg
Next

Jupyter = """"&OPENCOR_ROOT&"\Python\bin\Python.exe"" """&OPENCOR_ROOT&"\Python\Scripts\start_jupyter.py"" " & args

Set JupyterRun = shell.Exec(Jupyter)
Do Until JupyterRun.StdOut.AtEndOfStream
    Wscript.echo objExecObject.StdOut.ReadLine()
Loop
