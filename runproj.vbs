If CreateObject("Scripting.FileSystemObject").FileExists("build\bin\OpenCOR.exe") Then
    args = ""

    For Each arg in WScript.Arguments
        args = args&" "&arg
    Next

    CreateObject("WScript.Shell").Run "build\bin\OpenCOR.exe"&args, 1, False
Else
    WScript.Echo "OpenCOR must first be built before being run."
End If
