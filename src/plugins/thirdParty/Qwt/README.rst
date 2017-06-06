When building `Qwt <http://qwt.sourceforge.net/>`__, both `[OpenCOR]/build/ext/Build/QwtBuild/src/Makefile.Release` and `[OpenCOR]/build/ext/Build/QwtBuild/src/Makefile.Debug` get generated. Towards the top of those files, there are two lines that read:

    ::

      CC            = @echo compiling $< && cl
      CXX           = @echo compiling $< && cl

The problem with those two lines is that they will result in command lines that are too long for Windows. So, for the build to proceed, we need to replace them with:

    ::

      CC            = cl
      CXX           = cl
