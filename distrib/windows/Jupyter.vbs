' Make sure the path to Python is correct before starting Jupyter

UpdatePythonPath = "Python\bin\python.exe Python\Scripts\set_python_path.py Python -s"

UpdatePath =  "PATH=.;%PATH%"

' Start Jupyter

args = ""

For Each arg in WScript.Arguments
    args = args&" "&arg
Next

Jupyter = "Python\Scripts\jupyter.exe"&args

CreateObject("WScript.Shell").Run UpdatePythonPath & UpdatePath & Jupyter, 1, False
