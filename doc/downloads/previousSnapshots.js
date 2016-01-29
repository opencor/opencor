var jsonData = { "versions": [
                   { "major": 0, "minor": 0, "patch": 0, "day": 25, "month": 1, "year": 2016, "type": 2,
                     "platforms": [
                       { "name": "Windows", "supported": "Windows 7 and later",
                         "files": [
                           { "extension": ".exe" }, { "extension": ".zip" }
                         ]
                       },
                       { "name": "Linux", "supported": "Ubuntu 14.04 LTS (Trusty Tahr) and later",
                         "files": [
                           { "extension": ".tar.gz" },
                         ]
                       },
                       { "name": "OS X", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> 64-bit support only from now on (see issue <a href=\"https://github.com/opencor/opencor/issues/843\">#843</a>). Some SED-ML related refactoring (see issue <a href=\"https://github.com/opencor/opencor/issues/832\">#832</a>). Now use a user's proxy settings (see issue <a href=\"https://github.com/opencor/opencor/issues/845\">#845</a>)." },
                       { "change": "<strong>CellML support:</strong> the latest version of CellML is now used when creating a new CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/836\">#836</a>)." },
                       { "change": "<strong>Data stores:</strong> added support for <a href=\"http://biosignalml.org/\">BioSignalML</a> (work in progress)." },
                       { "change": "<strong>Viewer widget:</strong> improved/fixed the rendering of some equations (see issues <a href=\"https://github.com/opencor/opencor/issues/862\">#862</a> and <a href=\"https://github.com/opencor/opencor/issues/863\">#863</a>)." },
                       { "change": "<strong>CellML Text view:</strong> now require the user's agreement before saving a CellML file using a different CellML version (see issue <a href=\"https://github.com/opencor/opencor/issues/838\">#838</a>)." },
                       { "change": "<strong>Single Cell view:</strong> made the view more file specific (see issue <a href=\"https://github.com/opencor/opencor/issues/590\">#590</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://jquery.com/\">jQuery</a> to version 2.2.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/861\">#861</a>), <a href=\"http://www.llvm.org/\">LLVM</a> to version 3.7.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/859\">#859</a>) and <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2e (Windows only; see issue <a href=\"https://github.com/opencor/opencor/issues/840\">#840</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 25, "month": 11, "year": 2015, "type": 2,
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
                       { "change": "<strong>CellML support:</strong> don't distinguish between CellML 1.0 and CellML 1.1 anymore when creating a new CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/827\">#827</a>)." },
                       { "change": "<strong>SED-ML support:</strong> made it possible to create a new SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/826\">#826</a>)." },
                       { "change": "<strong>CellML Text view:</strong> hyphens and periods are now allowed in cmeta:id's (see issue <a href=\"https://github.com/opencor/opencor/issues/830\">#830</a>)." },
                       { "change": "<strong>Raw CellML view:</strong> fixed a problem with reformatting unescaping HTML entities (see issue <a href=\"https://github.com/opencor/opencor/issues/822\">#822</a>)." },
                       { "change": "<strong>Raw SED-ML view:</strong> this view has been added so that we can quickly edit and validate SED-ML files (see issue <a href=\"https://github.com/opencor/opencor/issues/682\">#682</a>)." },
                       { "change": "<strong>Single Cell view:</strong> disabled the development mode when using a read-only file (see issue <a href=\"https://github.com/opencor/opencor/issues/817\">#817</a>). Fixed a problem with the X axis sometimes not lining up when adding a new graph panel (see issue <a href=\"https://github.com/opencor/opencor/issues/820\">#820</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a> to version 5.12.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/812\">#812</a>) and <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.23.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/816\">#816</a>)." }
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
                       { "change": "<strong>Single Cell view:</strong> create a new or update an existing CellML file based on the results of a simulation (see issue <a href=\"https://github.com/opencor/opencor/issues/33\">#33</a>). Made the previous feature optional (see issue <a href=\"https://github.com/opencor/opencor/issues/810\">#810</a>). Respect CellML 1.1 model modularity (see issue <a href=\"https://github.com/opencor/opencor/issues/568\">#568</a>). Only export the relevant simulation data from CellML 1.1 files (see issue <a href=\"https://github.com/opencor/opencor/issues/808\">#808</a>). The X axis now always line up when using multiple graph panels (see issue <a href=\"https://github.com/opencor/opencor/issues/615\">#615</a>)." },
                       { "change": "<strong>Single Cell view + SED-ML:</strong> improved support on Windows. Export a simulation to a COMBINE Archive file (see issue <a href=\"https://github.com/opencor/opencor/issues/743\">#743</a>). Namespace prefixes used in XPath expressions are now defined (see issue <a href=\"https://github.com/opencor/opencor/issues/775\">#775</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://www.llvm.org/\">LLVM</a> to version 3.7.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/739\">#739</a>), <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.23.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/780\">#780</a>) and <a href=\"http://www.riverbankcomputing.co.uk/software/qscintilla/\">QScintilla</a> to version 2.9.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/798\">#798</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 27, "month": 9, "year": 2015, "type": 2,
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
                       { "change": "<strong>Development:</strong> upgraded to <a href=\"http://developer.apple.com/xcode/\">Xcode</a> 7.0." },
                       { "change": "<strong>General:</strong> improved Internet-related features (to prevent the <a href=\"http://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a> from hanging up unnecessarily) and the <a href=\"http://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>'s integrity (during lengthy processes, e.g. fully instantiating a <a href=\"http://www.cellml.org/\">CellML</a> 1.1 model), and added <a href=\"https://en.wikipedia.org/wiki/Zip_(file_format)\">ZIP</a> support (for our <a href=\"https://github.com/opencor/opencor/issues/743\">ongoing work</a> on supporting the <a href=\"http://co.mbine.org/documents/archive\">COMBINE Archive format</a>)." },
                       { "change": "<strong>Viewer widget:</strong> improved the rendering of mathematical equations and made it possible (on <a href=\"http://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>) to copy a rendered equation to the clipboard." },
                       { "change": "<strong>Single Cell view:</strong> improved the plotting of simulation data, as well as the interaction with a simulation and the export of a simulation to <a href=\"http://www.sed-ml.org/\">SED-ML</a>." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.23.2 and <a href=\"https://github.com/fbergmann/libSEDML/\">libSEDML</a> to version 0.3.1." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 7, "month": 9, "year": 2015, "type": 2,
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
                   { "major": 0, "minor": 0, "patch": 0, "day": 9, "month": 8, "year": 2015, "type": 2,
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
                   { "major": 0, "minor": 0, "patch": 0, "day": 15, "month": 7, "year": 2015, "type": 2,
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
                   { "major": 0, "minor": 0, "patch": 0, "day": 29, "month": 6, "year": 2015, "type": 2,
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
                   { "major": 0, "minor": 0, "patch": 0, "day": 9, "month": 6, "year": 2015, "type": 2,
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
                   }
                 ]
               };
