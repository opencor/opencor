AppExe = Left(Wscript.ScriptFullName, Len(Wscript.ScriptFullName)-Len(Wscript.ScriptName))&"build\bin\OpenCOR.exe"

If CreateObject("Scripting.FileSystemObject").FileExists(AppExe) Then
    Args = ""

    For Each Arg in WScript.Arguments
        Args = Args&" "&Arg
    Next

    Cmd = AppExe&Args

    CreateObject("WScript.Shell").Run Cmd, 1, False
Else
    WScript.Echo "OpenCOR must first be built before being run."
End If
