Set Shell = CreateObject("WScript.Shell")
AppDir = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)
UpdatePythonPath = AppDir&"\Python\bin\Python.exe "&AppDir&"\Python\Scripts\set_python_path.py "&AppDir&"\Python -s"

Shell.Run UpdatePythonPath, 0, True

Cmd = AppDir&"\Python\bin\Python.exe "&AppDir&"\Python\Scripts\start_jupyter.py lab --ip 127.0.0.1"

Shell.Run Cmd, 1, False
