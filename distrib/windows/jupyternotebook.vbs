shell = CreateObject("WScript.Shell")
homeDir = CreateObject("Scripting.FileSystemObject").GetParentFolderName(WScript.ScriptFullName)

updatePythonPath = """"&homeDir&"\Python\bin\Python.exe"" """&homeDir&"\Python\Scripts\set_python_path.py"" """&homeDir&"\Python"" -s"

shell.Run updatePythonPath, 0, True

cmd = """"&homeDir&"\Python\bin\Python.exe"" """&homeDir&"\Python\Scripts\start_jupyter.py"" notebook --ip 127.0.0.1"

shell.Run cmd, 1, False
