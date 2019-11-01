shell = CreateObject("WScript.Shell")
crtDir = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

updatePythonPath = """"&crtDir&"\Python\bin\Python.exe"" """&crtDir&"\Python\Scripts\set_python_path.py"" """&crtDir&"\Python"" -s"

shell.Run updatePythonPath, 0, True

cmd = """"&crtDir&"\Python\bin\Python.exe"" """&crtDir&"\Python\Scripts\start_jupyter.py"" lab --ip 127.0.0.1"

shell.Run cmd, 1, False
