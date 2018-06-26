If CreateObject("Scripting.FileSystemObject").FileExists("build\bin\OpenCOR.exe") Then
    args = ""

    For Each arg in WScript.Arguments
        args = args&" "&arg
    Next

    cmd = "build\bin\OpenCOR.exe"&args

    CreateObject("WScript.Shell").Run cmd, 1, False
Else
    WScript.Echo "OpenCOR must first be built before being run."
End If
