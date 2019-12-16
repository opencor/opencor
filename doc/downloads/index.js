var jsonData = { "versions": [
                   { "major": 0, "minor": 6, "patch": 0, "day": 11, "month": 12, "year": 2018, "type": 1, "license": 2,
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
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 13, "month": 12, "year": 2019, "type": 2, "license": 2,
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
                       { "change": "<strong>General:</strong> Replaced our use of <a href=\"https://developer.apple.com/library/archive/documentation/DeveloperTools/Conceptual/PackageMakerUserGuide/Introduction/Introduction.html\">PackageMaker</a> with that of productbuild on <a href=\"https://en.wikipedia.org/wiki/MacOS\">macOS</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/2210\">#2210</a>). A selectable plugin cannot be needed by another plugin (see issue <a href=\"https://github.com/opencor/opencor/issues/2247\">#2247</a>)." },
                       { "change": "<strong>Python support:</strong> added (initial) support for <a href=\"https://python.org/\">Python</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/1255\">#1255</a>). Added some <a href=\"https://python.org/\">Python</a>-based tests (see issue <a href=\"https://github.com/opencor/opencor/issues/2178\">#2178</a>). Now update the environment variable <code>PATH</code> using <code>\\</code> rather than <code>/</code> on Windows (see issue <a href=\"https://github.com/opencor/opencor/issues/2219\">#2219</a>). Merged our duplicated code (see issue <a href=\"https://github.com/opencor/opencor/issues/2225\">#2225</a>). Don't make our JupyterKernel and PythonShell plugins selectable (see issue <a href=\"https://github.com/opencor/opencor/issues/2245\">#2245</a>). Make sure that \"implementation\" and \"implementation_version\" are properly set in the JupyterKernel plugin (see issue <a href=\"https://github.com/opencor/opencor/issues/2250\">#2250</a>)." },
                       { "change": "<strong>CellML support:</strong> fixed an issue with the CellML API wrongly allowing code generation for non-runnable models (see issue <a href=\"https://github.com/opencor/opencor/issues/2240\">#2240</a>)." },
                       { "change": "<strong>SED-ML support:</strong> added support left and right triangle symbols (see issue <a href=\"https://github.com/opencor/opencor/issues/2173\">#2173</a>)." },
                       { "change": "<strong>Simulation support:</strong> don't require all our solvers to be installed anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/2234\">#2234</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> properly handle the Graph Panel properties (see issue <a href=\"https://github.com/opencor/opencor/issues/2171\">#2171</a>). Make sure that curves are plotted with all of their original settings when reloading a SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/2176\">#2176</a>). Make sure that all of a solver's properties get saved to a SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/2185\">#2185</a>). Don't access our central widget directly anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/2218\">#2218</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded the <a href=\"https://openssl.org/\">OpenSSL</a> library to version 1.1.1d (see issue <a href=\"https://github.com/opencor/opencor/issues/2157\">#2157</a>). Upgraded <a href=\"https://llvm.org/\">LLVM</a>+<a href=\"https://clang.llvm.org/\">Clang</a> to version 9.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/2161\">#2161</a>). Upgraded the <a href=\"https://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 5.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/2198\">#2198</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.11.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/2208\">#2208</a>). Upgraded <a href=\"https://mesa3d.org/\">Mesa</a> to version 19.2.6 (see issue <a href=\"https://github.com/opencor/opencor/issues/2230\">#2230</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 5, "patch": 0, "day": 15, "month": 10, "year": 2016, "type": 0, "license": 1,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 14.04 LTS (Trusty Tahr) and later",
                         "files": [
                           { "extension": ".tar.gz" }
                         ]
                       },
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 4, "patch": 1, "day": 20, "month": 5, "year": 2015, "type": 0, "license": 1,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe", "bitness": 32 }, { "extension": ".zip", "bitness": 32 },
                           { "extension": ".exe", "bitness": 64 }, { "extension": ".zip", "bitness": 64 }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 14.04 LTS (Trusty Tahr) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 4, "patch": 0, "day": 29, "month": 4, "year": 2015, "type": 0, "license": 1,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe", "bitness": 32 }, { "extension": ".zip", "bitness": 32 },
                           { "extension": ".exe", "bitness": 64 }, { "extension": ".zip", "bitness": 64 }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 14.04 LTS (Trusty Tahr) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 3, "patch": 0, "day": 19, "month": 8, "year": 2014, "type": 0, "license": 1,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows XP and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 14.04 LTS (Trusty Tahr) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 2, "patch": 0, "day": 19, "month": 11, "year": 2013, "type": 0, "license": 1,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows XP and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 12.04 LTS (Precise Pangolin) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 1, "patch": 2, "day": 29, "month": 5, "year": 2013, "type": 0, "license": 0,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows XP and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 12.04 LTS (Precise Pangolin) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 1, "patch": 1, "day": 17, "month": 4, "year": 2013, "type": 0, "license": 0,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows XP and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 12.04 LTS (Precise Pangolin) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 1, "patch": 0, "day": 1, "month": 4, "year": 2013, "type": 0, "license": 0,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows XP and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 12.04 LTS (Precise Pangolin) and later",
                         "files": [
                           { "extension": ".tar.gz", "bitness": 32 },
                           { "extension": ".tar.gz", "bitness": 64 }
                         ]
                       },
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   }
                 ]
               };
