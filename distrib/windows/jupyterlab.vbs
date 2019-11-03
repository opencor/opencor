Set shell = CreateObject("WScript.Shell")
Set appDir = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)
Set updatePythonPath = """"&appDir&"\Python\bin\Python.exe"" """&appDir&"\Python\Scripts\set_python_path.py"" """&appDir&"\Python"" -s"

shell.Run updatePythonPath, 0, True

Set cmd = """"&appDir&"\Python\bin\Python.exe"" """&appDir&"\Python\Scripts\start_jupyter.py"" lab --ip 127.0.0.1"

shell.Run cmd, 1, False
