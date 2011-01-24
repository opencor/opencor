args = ""

For Each arg in WScript.Arguments
    args = args&" "&arg
Next

CreateObject("WScript.Shell").Run "bin\OpenCOR.exe"&args, 1, False
