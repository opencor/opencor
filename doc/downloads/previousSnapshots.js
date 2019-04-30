var jsonData = { "versions": [
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
                       { "name": "macOS", "supported": "OS X 10.10 (Yosemite) and later",
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
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.11.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1983\">#1983</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.28.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1993\">#1993</a>). Upgraded <a href=\"https://www.mesa3d.org/\">Mesa</a> to version 18.3.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1994\">#1994</a>). Upgraded the <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 4.1.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1995\">#1995</a>). Upgraded the <a href=\"https://www.openssl.org/\">OpenSSL</a> library to version 1.0.2r (see issue <a href=\"https://github.com/opencor/opencor/issues/2009\">#2009</a>). Upgraded <a href=\"https://katex.org/\">KaTeX</a> to version 0.10.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/2014\">#2014</a>)." }
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
                       { "name": "macOS", "supported": "OS X 10.10 (Yosemite) and later",
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
                       { "change": "<strong>CellML tools:</strong> can now validate a <a href=\"https://www.cellml.org/\">CellML</a> file from the <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/1964\">#1964</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> added basic support for the import of external data (see issue <a href=\"https://github.com/opencor/opencor/issues/1845\">#1845</a>). Ensure that graph panels are always properly aligned (see issue <a href=\"https://github.com/opencor/opencor/issues/1956\">#1956</a>). Fixed various issues with saving a file under a new name (see issue <a href=\"https://github.com/opencor/opencor/issues/1967\">#1967</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 7.0.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1917\">#1917</a>). Upgraded the <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 4.0.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/1960\">#1960</a>). Upgraded the <a href=\"http://qwt.sourceforge.net/\">Qwt</a> library to version 6.1.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1933\">#1933</a>). Upgraded <a href=\"https://www.mesa3d.org/\">Mesa</a> to version 18.2.8 (see issue <a href=\"https://github.com/opencor/opencor/issues/1945\">#1945</a>)." }
                     ]
                   }
                 ]
               };
