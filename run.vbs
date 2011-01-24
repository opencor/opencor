If CreateObject("Scripting.FileSystemObject").FileExists("build\OpenCOR.exe") Then
    args = ""

    For Each arg in WScript.Arguments
        args = args&" "&arg
    Next

    CreateObject("WScript.Shell").Run "build\OpenCOR.exe"&args, 1, False
Else
    WScript.Echo "Information: OpenCOR must first be built before being run."
End If
