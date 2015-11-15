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
                   { "major": 0, "minor": 0, "patch": 0, "day": 15, "month": 11, "year": 2015, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>Development:</strong> upgraded to <a href=\"http://developer.apple.com/xcode/\">Xcode</a> 7.1.1." },
                       { "change": "<strong>Supported platforms:</strong> make sure that the main window makes use of the screen estate on Windows 10 (see issue <a href=\"https://github.com/opencor/opencor/issues/788\">#788</a>). Fixed an issue with some parts of the GUI being black on OS X 10.11 (El Capitan; see issue <a href=\"https://github.com/opencor/opencor/issues/779\">#779</a>)." },
                       { "change": "<strong>General:</strong> slight improvement to our plugin interface. Allow one or several CLI versions of OpenCOR to run along the GUI version (see issue <a href=\"https://github.com/opencor/opencor/issues/803\">#803</a>). Fixed a small issue with the reopening of most recent files (see issue <a href=\"https://github.com/opencor/opencor/issues/770\">#770</a>). New file menus are now always available (see issue <a href=\"https://github.com/opencor/opencor/issues/805\">#805</a>)" },
                       { "change": "<strong>CellML support:</strong> improved the support for arbitrary logarithm (see issue <a href=\"https://github.com/opencor/opencor/issues/791\">#791</a>)." },
                       { "change": "<strong>CellML Annotation view:</strong> fixed an issue with looking up an unknown term when viewing a locked file (see issue <a href=\"https://github.com/opencor/opencor/issues/765\">#765</a>)." },
                       { "change": "<strong>Viewer widget:</strong> allow for both capital and minuscule Greek characters (see issue <a href=\"https://github.com/opencor/opencor/issues/773\">#773</a>). Don't generate unnecessary parentheses (see issue <a href=\"https://github.com/opencor/opencor/issues/774\">#774</a>) and generate some missing ones (see issue <a href=\"https://github.com/opencor/opencor/issues/784\">#784</a>). Better render superscripts and subscripts (see issue <a href=\"https://github.com/opencor/opencor/issues/782\">#782</a>). Improved the rendering of various operators (see issue <a href=\"https://github.com/opencor/opencor/issues/797\">#797</a>)." },
                       { "change": "<strong>Editor widget:</strong> highlight matching braces (see issue <a href=\"https://github.com/opencor/opencor/issues/786\">#786</a>)." },
                       { "change": "<strong>CellML Text view:</strong> added support for some trigonometric operators, <code>min(a, b, ...)</code> and <code>max(a, b, ...)</code> (see issue <a href=\"https://github.com/opencor/opencor/issues/789\">#789</a>), and <code>gcd(a, b, ...)</code> and <code>lcm(a, b, ...)</code> (see issue <a href=\"https://github.com/opencor/opencor/issues/790\">#790</a>)." },
                       { "change": "<strong>Single Cell (simulation) view:</strong> create a new or update an existing CellML file based on the results of a simulation (see issue <a href=\"https://github.com/opencor/opencor/issues/33\">#33</a>). Made the previous feature optional (see issue <a href=\"https://github.com/opencor/opencor/issues/810\">#810</a>). Respect CellML 1.1 model modularity (see issue <a href=\"https://github.com/opencor/opencor/issues/568\">#568</a>). Only export the relevant simulation data from CellML 1.1 files (see issue <a href=\"https://github.com/opencor/opencor/issues/808\">#808</a>). The X-axis now always line up when using multiple graph panels (see issue <a href=\"https://github.com/opencor/opencor/issues/615\">#615</a>)." },
                       { "change": "<strong>Single Cell (simulation) view + SED-ML:</strong> improved support on Windows. Export a simulation to a COMBINE Archive file (see issue <a href=\"https://github.com/opencor/opencor/issues/743\">#743</a>). Namespace prefixes used in XPath expressions are now defined (see issue <a href=\"https://github.com/opencor/opencor/issues/775\">#775</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://www.llvm.org/\">LLVM</a> to version 3.7.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/739\">#739</a>), <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.23.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/780\">#780</a>) and <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a> to version 2.9.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/798\">#798</a>)." }
                     ],
                     "information": "<strong>Note:</strong> this snapshot is for those who want/need access to OpenCOR's latest features, meaning that it may be unstable."
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
