var jsonData = { "versions": [
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
