If CreateObject("Scripting.FileSystemObject").FileExists("build\OpenCOR.qhc") And _
   CreateObject("Scripting.FileSystemObject").FileExists("build\OpenCOR.qch") Then
    CreateObject("WScript.Shell").Run "assistant -collectionFile build\OpenCOR.qhc", 1, False
Else
    WScript.Echo "Warning: OpenCOR must first be built before seeing its help."
End If
