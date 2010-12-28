If CreateObject("Scripting.FileSystemObject").FileExists("build\OpenCOR.exe") Then
    CreateObject("WScript.Shell").Run "build\OpenCOR.exe", 1, False
Else
    WScript.Echo "Warning: OpenCOR must first be built before being run."
End If
