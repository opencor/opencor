var jsonData = { "versions": [
                   { "major": 0, "minor": 0, "patch": 0, "day": 9, "month": 9, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> prevent OpenCOR from crashing when on Windows, maximised and trying to open a dialog (see issue <a href=\"https://github.com/opencor/opencor/issues/2154\">#2154</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://llvm.org/\">LLVM</a>+<a href=\"https://clang.llvm.org/\">Clang</a> to version 8.0.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/2147\">#2147</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.28.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/2148\">#2148</a>). Upgraded <a href=\"https://mesa3d.org/\">Mesa</a> to version 19.1.5 (see issue <a href=\"https://github.com/opencor/opencor/issues/2149\">#2149</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 19, "month": 7, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> make sure that we have a view before trying to import a file using OpenCOR's <a href=\"https://en.wikipedia.org/wiki/Uniform_Resource_Identifier\">URI scheme</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/2140\">#2140</a>)." },
                       { "change": "<strong>Simulation support:</strong> improved the accuracy of algebraic variables that depend on the value of some state variables (see issue <a href=\"https://github.com/opencor/opencor/issues/2139\">#2139</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded the <a href=\"https://openssl.org/\">OpenSSL</a> library to version 1.1.1c (see issue <a href=\"https://github.com/opencor/opencor/issues/1801\">#1801</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 27, "month": 6, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> prebuilt third-party libraries are now installed under <a href=\"https://github.com/opencor/opencor/tree/master/ext\"><code>[OpenCOR]/ext</code></a> (see issue <a href=\"https://github.com/opencor/opencor/issues/2126\">#2126</a>)." },
                       { "change": "<strong>CellML Text view:</strong> can once again read a CellML file that has no XML processing instruction (see issue <a href=\"https://github.com/opencor/opencor/issues/2125\">#2125</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> properly disable the `Reset All Model Parameters` button (see issue <a href=\"https://github.com/opencor/opencor/issues/2116\">#2116</a>). Fixed an issue with the plotting of simulation data being potentially (very) choppy (see issue <a href=\"https://github.com/opencor/opencor/issues/2118\">#2118</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 3.4.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/2122\">#2122</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.28.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/2123\">#2123</a>). Upgraded <a href=\"https://mesa3d.org/\">Mesa</a> to version 19.1.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/2124\">#2124</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.11.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/2131\">#2131</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 17, "month": 6, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> support <a href=\"https://en.wikipedia.org/wiki/Ubuntu_version_history#Ubuntu_19.04_(Disco_Dingo)\">Ubuntu 19.04</a> by shipping <a href=\"https://packages.ubuntu.com/bionic/libtinfo5\">libtinfo.so.5</a> on Linux (see issue <a href=\"https://github.com/opencor/opencor/issues/2075\">#2075</a>). now return the exit code of a <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a> plugin command (see issue <a href=\"https://github.com/opencor/opencor/issues/2107\">#2107</a>)." },
                       { "change": "<strong>CellML support:</strong> improved the validation reporting of models with imports (see issue <a href=\"https://github.com/opencor/opencor/issues/2109\">#2109</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded the <a href=\"https://openssl.org/\">OpenSSL</a> library to version 1.0.2s (see issue <a href=\"https://github.com/opencor/opencor/issues/2105\">#2105</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 23, "month": 5, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> fixed an issue with <a href=\"https://qt.io/\">Qt</a> not being able to find its platform plugin anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/2089\">#2089</a>)." },
                       { "change": "<strong>Simulation support:</strong> allow a remote <a href=\"https://sed-ml.github.io/\">SED-ML</a> file to be run headless (see issue <a href=\"https://github.com/opencor/opencor/issues/2085\">#2085</a>)." },
                       { "change": "<strong>Help window:</strong> fixed and issue with a blank page being shown rather than the documentation itself (see issue <a href=\"https://github.com/opencor/opencor/issues/2099\">#2099</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> fixed an issue with <a href=\"https://libgit2.github.com/\">libgit2</a> referencing two different copies of <a href=\"https://zlib.net/\">zlib</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/2088\">#2088</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 13, "month": 5, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>SED-ML support:</strong> fixed our detection of SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/2082\">#2082</a>)." },
                       { "change": "<strong>Simulation support:</strong> generalised the reporting of a simulation's issues (see issue <a href=\"https://github.com/opencor/opencor/issues/2080\">#2080</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 5, "month": 5, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 18.04 LTS (Bionic Beaver) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> now rely on <a href=\"https://visualstudio.microsoft.com/downloads/\">Visual Studio 2017</a> to build OpenCOR on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/2041\">#2041</a>). Upgraded our minimum requirement to <a href=\"https://en.wikipedia.org/wiki/Ubuntu_version_history#Ubuntu_18.04_LTS_(Bionic_Beaver)\">Ubuntu 18.04 LTS</a> on Linux (see issue <a href=\"https://github.com/opencor/opencor/issues/2050\">#2050</a>). Renamed QScintillaSupport to QScintillaWidget (see issue <a href=\"https://github.com/opencor/opencor/issues/2066\">#2066</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> first list changed files and then show the user's comment (in the synchronisation dialog; see issue <a href=\"https://github.com/opencor/opencor/issues/2065\">#2065</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://llvm.org/\">LLVM</a>+<a href=\"https://clang.llvm.org/\">Clang</a> to version 8.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/2040\">#2040</a>). Upgraded <a href=\"https://github.com/fbergmann/libSEDML/\">libSEDML</a> to version 0.4.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/2042\">#2042</a>). Upgraded <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a> to version 5.18.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/2064\">#2064</a>). Upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 3.4.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/2068\">#2068</a>). Upgraded <a href=\"https://mesa3d.org/\">Mesa</a> to version 19.0.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/2070\">#2070</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 7, "month": 3, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 16.04 LTS (Xenial Xerus) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> enforce the use of Light Mode on <a href=\"https://en.wikipedia.org/wiki/MacOS\">macOS</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/1991\">#1991</a>). Added the <code>importFile</code> and <code>importFiles</code> commands to OpenCOR's <a href=\"https://en.wikipedia.org/wiki/Uniform_Resource_Identifier\">URI scheme</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/2000\">#2000</a>). OpenCOR's geometry is now properly restored on macOS when there is at least one remote file (see issue <a href=\"https://github.com/opencor/opencor/issues/2030\">#2030</a>)." },
                       { "change": "<strong>CSV data store:</strong> improved our detection of CSV files (see issue <a href=\"https://github.com/opencor/opencor/issues/2019\">#2019</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> make sure that a folder is writable (see issue <a href=\"https://github.com/opencor/opencor/issues/2032\">#2032</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> fixed an issue with a <a href=\"https://sed-ml.github.io/\">SED-ML</a> file / <a href=\"https://co.mbine.org/documents/archive\">COMBINE archive</a> being considered modified after changing languages (see issue <a href=\"https://github.com/opencor/opencor/issues/2001\">#2001</a>). Make some widgets look properly disabled when they are (see issue <a href=\"https://github.com/opencor/opencor/issues/2022\">#2022</a>). Properly handle the import of the same data in different files (see issue <a href=\"https://github.com/opencor/opencor/issues/2028\">#2028</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.11.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1983\">#1983</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.28.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1993\">#1993</a>). Upgraded <a href=\"https://mesa3d.org/\">Mesa</a> to version 18.3.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1994\">#1994</a>). Upgraded the <a href=\"https://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 4.1.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1995\">#1995</a>). Upgraded the <a href=\"https://openssl.org/\">OpenSSL</a> library to version 1.0.2r (see issue <a href=\"https://github.com/opencor/opencor/issues/2009\">#2009</a>). Upgraded <a href=\"https://katex.org/\">KaTeX</a> to version 0.10.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/2014\">#2014</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 1, "month": 2, "year": 2019, "type": 2, "license": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 16.04 LTS (Xenial Xerus) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "macOS 10.12 (Sierra) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> upgraded to Qt 5.12.0 LTS (see issue <a href=\"https://github.com/opencor/opencor/issues/1910\">#1910</a>)." },
                       { "change": "<strong>COMBINE support:</strong> now recognise files that have a master attribute of \"1\" rather than \"true\" (see issue <a href=\"https://github.com/opencor/opencor/issues/1912\">#1912</a>)." },
                       { "change": "<strong>Graph Panel widget:</strong> fixed a small problem with the scale range of an axis in some cases (see issue <a href=\"https://github.com/opencor/opencor/issues/1941\">#1941</a>)." },
                       { "change": "<strong>CellML Annotation view:</strong> prevent OpenCOR from crashing after a CellML file has been rendered invalid (see issue <a href=\"https://github.com/opencor/opencor/issues/1958\">#1958</a>)." },
                       { "change": "<strong>CellML Text view:</strong> improved Unicode support (see issue <a href=\"https://github.com/opencor/opencor/issues/1926\">#1926</a>)." },
                       { "change": "<strong>CellML tools:</strong> can now validate a <a href=\"https://cellml.org/\">CellML</a> file from the <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/1964\">#1964</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> added basic support for the import of external data (see issue <a href=\"https://github.com/opencor/opencor/issues/1845\">#1845</a>). Ensure that graph panels are always properly aligned (see issue <a href=\"https://github.com/opencor/opencor/issues/1956\">#1956</a>). Fixed various issues with saving a file under a new name (see issue <a href=\"https://github.com/opencor/opencor/issues/1967\">#1967</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://llvm.org/\">LLVM</a>+<a href=\"https://clang.llvm.org/\">Clang</a> to version 7.0.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1917\">#1917</a>). Upgraded the <a href=\"https://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 4.0.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/1960\">#1960</a>). Upgraded the <a href=\"http://qwt.sourceforge.net/\">Qwt</a> library to version 6.1.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1933\">#1933</a>). Upgraded <a href=\"https://mesa3d.org/\">Mesa</a> to version 18.2.8 (see issue <a href=\"https://github.com/opencor/opencor/issues/1945\">#1945</a>)." }
                     ]
                   }
                 ]
               };
