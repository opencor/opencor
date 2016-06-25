Qt WebKit has been officially replaced with Qt WebEngine starting with Qt 5.6 LTS. However, Qt WebEngine is still lacking some features, making it impossible for us to switch to it just yet. Our original plan was therefore to stick with the Qt 5.5 branch, but it has known issues (e.g. https://bugreports.qt.io/browse/QTBUG-46882) and they will never be addressed since that branch if not being maintained anymore. We therefore decided to switch to the Qt 5.6 LTS, which means ahving to build our own copy of Qt WebKit. This requires the following:
 - Qt WebKit's core source code (see http://download.qt.io/community_releases/5.6/5.6.1/); and
 - Making sure that we are all set to to build Qt WebKit on:
    - Windows (see https://github.com/annulen/webkit/wiki/Building-QtWebKit-on-Windows);
    - Linux (see https://github.com/annulen/webkit/wiki/Building-QtWebKit-on-Linux); and
    - OS X (see https://github.com/annulen/webkit/wiki/Building-QtWebKit-on-OS-X).

From there, using cmake-gui on Windows or ccmake on Linux / OS X, we want to:
 $ qmake
 $ make
