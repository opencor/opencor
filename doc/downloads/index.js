var jsonData = { "versions": [
                   { "major": 0, "minor": 4, "patch": 1, "day": 20, "month": 5, "year": 2015, "type": 1,
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
                       { "name": "OS X", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 9, "month": 6, "year": 2016, "type": 2,
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
                       { "name": "OS X", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> merged and renamed some of our plugins (see issue <a href=\"https://github.com/opencor/opencor/issues/1004\">#1004</a>). Upgraded to Qt 5.6.1 LTS + Qt WebKit (see issue <a href=\"https://github.com/opencor/opencor/issues/1015\">#1015</a>). A new file is not automatically considered as modified anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/1020\">#1020</a>)." },
                       { "change": "<strong>CellML support:</strong> can now detect a variable of integration no matter where it is defined in a model (see issue <a href=\"https://github.com/opencor/opencor/issues/995\">#995</a>)." },
                       { "change": "<strong>Data stores:</strong> data export is now done in its own thread (ee issue <a href=\"https://github.com/opencor/opencor/issues/849\">#849</a>)." },
                       { "change": "<strong>Text-based editing views:</strong> fixed a small problem with losing the focus and not being able to track changes to a file after having reloaded another file that depends on it (see issue <a href=\"https://github.com/opencor/opencor/issues/996\">#996</a>)." },
                       { "change": "<strong>CellML Text view:</strong> allow the import / export of the CellML Text format from / to CellML from the command line (see issue <a href=\"https://github.com/opencor/opencor/issues/992\">#992</a>). Allow the user to save the contents of the view to a text file if it cannot be saved as a CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/998\">#998</a>)." },
                       { "change": "<strong>Single Cell view:</strong> now use better colours/symbols in the Parameters window (see issue <a href=\"https://github.com/opencor/opencor/issues/634\">#634</a>). Can now specify the minimum/maximum values of the axes of a graph panel (see issue <a href=\"https://github.com/opencor/opencor/issues/997\">#997</a>). Can now synchronise the X and/or Y axes of all the graph panels (see issue <a href=\"https://github.com/opencor/opencor/issues/1000\">#1000</a>). The plotting of several graph panels is now much smoother (see issue <a href=\"https://github.com/opencor/opencor/issues/1011\">#1011</a>). Fixed a problem with some model parameters not being listed or being listed in the wrong component (see issues <a href=\"https://github.com/opencor/opencor/issues/1014\">#1014</a> and <a href=\"https://github.com/opencor/opencor/issues/1016\">#1016</a>). Improved the loading/reloading of a file (see issue <a href=\"https://github.com/opencor/opencor/issues/1017\">#1017</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> applied an official patch to <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> 2.9.2 following a (Mac) OS X issue (see issue <a href=\"https://github.com/opencor/opencor/issues/994\">#994</a>). Upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 3.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1001\">#1001</a>) and <a href=\"http://qwt.sourceforge.net/\">Qwt</a> to version 6.1.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/1002\">#1002</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 4, "patch": 0, "day": 29, "month": 4, "year": 2015, "type": 0,
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
                       { "name": "OS X", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 3, "patch": 0, "day": 19, "month": 8, "year": 2014, "type": 0,
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
                       { "name": "OS X", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 2, "patch": 0, "day": 19, "month": 11, "year": 2013, "type": 0,
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
                       { "name": "OS X", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 1, "patch": 2, "day": 29, "month": 5, "year": 2013, "type": 0,
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
                       { "name": "OS X", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 1, "patch": 1, "day": 17, "month": 4, "year": 2013, "type": 0,
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
                       { "name": "OS X", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   },
                   { "major": 0, "minor": 1, "patch": 0, "day": 1, "month": 4, "year": 2013, "type": 0,
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
                       { "name": "OS X", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   }
                 ]
               };
