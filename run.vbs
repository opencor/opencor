Set AppExe = Left(Wscript.ScriptFullName, Len(Wscript.ScriptFullName)-Len(Wscript.ScriptName))&"build\bin\OpenCOR.exe"

If CreateObject("Scripting.FileSystemObject").FileExists(AppExe) Then
    Set args = ""

    For Each arg in WScript.Arguments
        args = args&" "&arg
    Next

    Set cmd = """"&AppExe&args&""""

    CreateObject("WScript.Shell").Run cmd, 1, False
Else
    WScript.Echo "OpenCOR must first be built before being run."
End If
