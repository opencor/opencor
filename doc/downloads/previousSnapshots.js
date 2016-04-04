var jsonData = { "versions": [
                   { "major": 0, "minor": 0, "patch": 0, "day": 31, "month": 3, "year": 2016, "type": 2,
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
                       { "change": "<strong>General:</strong> replaced our minimal <em>How to get started?</em> documentation with a link to Peter Hunter's <a href=\"http://tutorial-on-cellml-opencor-and-pmr.readthedocs.org/en/latest/index.html\">comprehensive tutorial</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/654\">#654</a>). Now detect when one of a file's dependencies has been modified and offer to reload the file (see issue <a href=\"https://github.com/opencor/opencor/issues/887\">#887</a>). Fixed a problem with OpenCOR firing up an 'old' version of OS X, but not opening the CellML/SED-ML file referenced in an OpenCOR URL (see issue <a href=\"https://github.com/opencor/opencor/issues/909\">#909</a>). Now use the <a href=\"https://code.qt.io/cgit/qt-solutions/qt-solutions.git/\">Qt Solutions</a> version of <a href=\"https://code.qt.io/cgit/qt-solutions/qt-solutions.git/tree/qtsingleapplication\">QtSingleApplication</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/916\">#916</a>). Now use a preferred view to open a particular type of file (see issue <a href=\"https://github.com/opencor/opencor/issues/925\">#925</a>). Now, by default, we open a file using the ‘All Files’ filter, while we use the first supported filter when saving a file (see issue <a href=\"https://github.com/opencor/opencor/issues/927\">#927</a>)." },
                       { "change": "<strong>Single Cell view:</strong> fixed a problem with OpenCOR crashing when saving a 'new' CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/919\">#919</a>). Fixed a problem with boolean solver properties not being correctly retrieved from a SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/920\">#920</a>). Fixed a problem with opening a CellML model from a SED-ML file not working when there are spaces in the file name (see issue <a href=\"https://github.com/opencor/opencor/issues/922\">#922</a>). Now handle the case where a SED-ML file is generated on a different drive from the CellML file it refers (see issue <a href=\"https://github.com/opencor/opencor/issues/928\">#928</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 2.2.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/924\">#924</a>), <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.24 (see issue <a href=\"https://github.com/opencor/opencor/issues/914\">#914</a>) and <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2g (Windows only; see issue <a href=\"https://github.com/opencor/opencor/issues/910\">#910</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 27, "month": 2, "year": 2016, "type": 2,
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
                       { "change": "<strong>General:</strong> refactored the handling of OpenCOR actions (see issue <a href=\"https://github.com/opencor/opencor/issues/901\">#901</a>). Added support for an OpenCOR-specific URL scheme (see issue <a href=\"https://github.com/opencor/opencor/issues/903\">#903</a>). Fixed a problem that crashed OpenCOR on Linux due to an external function not being found by LLVM's execution engine (see <a href=\"https://github.com/opencor/opencor/issues/904\">#904</a>)" },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 2.2.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/902\">#902</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 22, "month": 2, "year": 2016, "type": 2,
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
                       { "change": "<strong>General:</strong> can now handle 3xx redirection headers (see issue <a href=\"https://github.com/opencor/opencor/issues/893\">#893</a>)." },
                       { "change": "<strong>Single Cell view:</strong> fixed a problem with opening a remote SED-ML file not working with relatively specified sources (see issue <a href=\"https://github.com/opencor/opencor/issues/894\">#894</a>). Can now open a CellML file referenced in a SED-ML file or COMBINE archive (see issue <a href=\"https://github.com/opencor/opencor/issues/895\">#895</a>). Fixed a small problem with the URL of a model being wrongly formatted on Windows (see issue <a href=\"https://github.com/opencor/opencor/issues/896\">#896</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 19, "month": 2, "year": 2016, "type": 2,
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
                       { "change": "<strong>General:</strong> improved the retrieval of a save file name (see issue <a href=\"https://github.com/opencor/opencor/issues/881\">#881</a>). Made it possible to detect file changes in binary files (see issue <a href=\"https://github.com/opencor/opencor/issues/882\">#882</a>). All of OpenCOR now gets disabled when the central widget's busy widget is showing (see issue <a href=\"https://github.com/opencor/opencor/issues/890\">#890</a>)." },
                       { "change": "<strong>CellML support:</strong> now warn the user that an issue may be related to an (in)directly imported CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/888\">#888</a>)." },
                       { "change": "<strong>Raw Text view:</strong> only allow text-based files (see issue <a href=\"https://github.com/opencor/opencor/issues/884\">#884</a>). Formerly known as the Raw view (see issue <a href=\"https://github.com/opencor/opencor/issues/885\">#885</a>)." },
                       { "change": "<strong>Single Cell view:</strong> can now open and run a 'simple' SED-ML file / COMBINE archive (see issue <a href=\"https://github.com/opencor/opencor/issues/825\">#825</a>). Fixed a small problem with the progress bar not always getting reset (see issue <a href=\"https://github.com/opencor/opencor/issues/879\">#879</a>). Fixed a small problem with renaming a file (see commit <a href=\"https://github.com/opencor/opencor/commit/e8760b12dc52e86a8993cbe3cc9e8ee65d7b54cb\">e8760b1</a>). Fixed a small problem following our work on issue <a href=\"https://github.com/opencor/opencor/issues/590\">#590</a> (see commits <a href=\"https://github.com/opencor/opencor/commit/f8fb16b4f5c481463c804ac0fe28beb8f7d23f2a\">f8fb16b</a> and <a href=\"https://github.com/opencor/opencor/commit/93001bcada3192c33bf6a3562a4c89bd2bb1f272\">93001bc</a>). Make it accept the SED-ML and COMBINE MIME types (see commit <a href=\"https://github.com/opencor/opencor/commit/f92e21f979aab64df21047fa414031ff9627af70\">f92e21f</a>; somewhat related to issue <a href=\"https://github.com/opencor/opencor/issues/891\">#891</a>). Can now duplicate a CellML file, SED-ML file and COMBINE archive, and save it (see commit <a href=\"https://github.com/opencor/opencor/commit/f3c1710d042877375f91e58f72b27a391fa285e3\">f3c1710</a>). Revisited the loading/saving of some settings (see commits <a href=\"https://github.com/opencor/opencor/commit/2b727d8875f983b13287870602dd0eaaf6c78480\">2b727d8</a>, <a href=\"https://github.com/opencor/opencor/commit/b44dba6d8f9d4c848b0fff2558ef8f041c16cad6\">b44dba6</a>, <a href=\"https://github.com/opencor/opencor/commit/15bfdb7fe24c5f5f247d6ffcd61306dfad77e3af\">15bfdb7</a>, <a href=\"https://github.com/opencor/opencor/commit/6bbe034959ddd7479f2a5e70ab58e9039463d8dd\">6bbe034</a> and <a href=\"https://github.com/opencor/opencor/commit/35f50b4d436cbdfd784973dc0a9978ff7337d1ae\">35f50b4</a>). Fixed a small problem with the creation of variable targets (see commit <a href=\"https://github.com/opencor/opencor/commit/fab6f9d7d1f618e1e1736f7ab1a9b7f37d0cb05f\">fab6f9d</a>)." },
                       { "change": "<strong>Miscellaneous:</strong> kind of reverted commit <a href=\"https://github.com/opencor/opencor/commit/59f3ce318fe9eb8f5ee9f3d24c79186e56e31c1f\">59f3ce3</a> (related to issue <a href=\"https://github.com/opencor/opencor/issues/868\">#868</a>) (see commit <a href=\"https://github.com/opencor/opencor/commit/8a1da13bbf08ff617b84af2b235c73bdb1cd9625\">8a1da13</a>). Show our busy widget when reloading an indirect remote file (see commits <a href=\"https://github.com/opencor/opencor/commit/0b8290559e5b0d7334e2972a4cb6ed6f630f6e31\">0b82905</a>, <a href=\"https://github.com/opencor/opencor/commit/3eaea771e702a648bff4dd424af9e044f5fb755a\">3eaea77</a>, <a href=\"https://github.com/opencor/opencor/commit/431e361f4883035dccfa98a436ce54bf0fb4e0a7\">431e361</a> and <a href=\"https://github.com/opencor/opencor/commit/5dbe5aeb527c461b7a7e51a25dd8bc48c10dcca2\">5dbe5ae</a>). Fixed a regression issue with regards to retrieving the filters to use to open/save a file (see commit <a href=\"https://github.com/opencor/opencor/commit/f94555468760f5243c4ba46910bac81df606d158\">f945554</a>). Revised <code>StandardFileManager::isFile()</code> so that it doesn't consider new files being necessarily of the right type (useful when duplicating a file; see commits <a href=\"https://github.com/opencor/opencor/commit/1fb253ffb0392d37c35966a34634e3225a2e8ede\">1fb253f</a> and <a href=\"https://github.com/opencor/opencor/commit/04e965c9140697a93091b1fe8ab70114ebf06205\">04e965c</a>; related to issue <a href=\"https://github.com/opencor/opencor/issues/891\">#891</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 1, "month": 2, "year": 2016, "type": 2,
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
                       { "change": "<strong>Viewer widget:</strong> fixed the rendering of some equations (see issue <a href=\"https://github.com/opencor/opencor/issues/869\">#869</a>)." },
                       { "change": "<strong>Single Cell view:</strong> fixed some regression issues following our work on issue <a href=\"https://github.com/opencor/opencor/issues/590\">#590</a> (see issues <a href=\"https://github.com/opencor/opencor/issues/866\">#866</a>, <a href=\"https://github.com/opencor/opencor/issues/868\">#868</a> and <a href=\"https://github.com/opencor/opencor/issues/876\">#876</a>). Fixed a small problem with the progress bar not always getting reset (see issue <a href=\"https://github.com/opencor/opencor/issues/874\">#874</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2f (Windows only; see issue <a href=\"https://github.com/opencor/opencor/issues/870\">#870</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 25, "month": 1, "year": 2016, "type": 2,
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
                       { "change": "<strong>General:</strong> 64-bit support only from now on (see issue <a href=\"https://github.com/opencor/opencor/issues/843\">#843</a>). Some SED-ML related refactoring (see issue <a href=\"https://github.com/opencor/opencor/issues/832\">#832</a>). Now use a user's proxy settings (see issue <a href=\"https://github.com/opencor/opencor/issues/845\">#845</a>)." },
                       { "change": "<strong>CellML support:</strong> the latest version of CellML is now used when creating a new CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/836\">#836</a>)." },
                       { "change": "<strong>Data stores:</strong> added support for BioSignalML (work in progress)." },
                       { "change": "<strong>Viewer widget:</strong> improved/fixed the rendering of some equations (see issues <a href=\"https://github.com/opencor/opencor/issues/862\">#862</a> and <a href=\"https://github.com/opencor/opencor/issues/863\">#863</a>)." },
                       { "change": "<strong>CellML Text view:</strong> now require the user's agreement before saving a CellML file using a different CellML version (see issue <a href=\"https://github.com/opencor/opencor/issues/838\">#838</a>)." },
                       { "change": "<strong>Single Cell view:</strong> made the view more file specific (see issue <a href=\"https://github.com/opencor/opencor/issues/590\">#590</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 2.2.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/861\">#861</a>), <a href=\"http://www.llvm.org/\">LLVM</a> to version 3.7.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/859\">#859</a>) and <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2e (Windows only; see issue <a href=\"https://github.com/opencor/opencor/issues/840\">#840</a>)." }
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
                       { "change": "<strong>Development:</strong> upgraded to <a href=\"https://developer.apple.com/xcode/\">Xcode</a> 7.1.1." },
                       { "change": "<strong>Supported platforms:</strong> make sure that the main window makes use of the screen estate on Windows 10 (see issue <a href=\"https://github.com/opencor/opencor/issues/788\">#788</a>). Fixed an issue with some parts of the GUI being black on OS X 10.11 (El Capitan; see issue <a href=\"https://github.com/opencor/opencor/issues/779\">#779</a>)." },
                       { "change": "<strong>General:</strong> slight improvement to our plugin interface. Allow one or several CLI versions of OpenCOR to run along the GUI version (see issue <a href=\"https://github.com/opencor/opencor/issues/803\">#803</a>). Fixed a small issue with the reopening of most recent files (see issue <a href=\"https://github.com/opencor/opencor/issues/770\">#770</a>). New file menus are now always available (see issue <a href=\"https://github.com/opencor/opencor/issues/805\">#805</a>)" },
                       { "change": "<strong>CellML support:</strong> improved the support for arbitrary logarithm (see issue <a href=\"https://github.com/opencor/opencor/issues/791\">#791</a>)." },
                       { "change": "<strong>CellML Annotation view:</strong> fixed an issue with looking up an unknown term when viewing a locked file (see issue <a href=\"https://github.com/opencor/opencor/issues/765\">#765</a>)." },
                       { "change": "<strong>Viewer widget:</strong> allow for both capital and minuscule Greek characters (see issue <a href=\"https://github.com/opencor/opencor/issues/773\">#773</a>). Don't generate unnecessary parentheses (see issue <a href=\"https://github.com/opencor/opencor/issues/774\">#774</a>) and generate some missing ones (see issue <a href=\"https://github.com/opencor/opencor/issues/784\">#784</a>). Better render superscripts and subscripts (see issue <a href=\"https://github.com/opencor/opencor/issues/782\">#782</a>). Improved the rendering of various operators (see issue <a href=\"https://github.com/opencor/opencor/issues/797\">#797</a>)." },
                       { "change": "<strong>Editor widget:</strong> highlight matching braces (see issue <a href=\"https://github.com/opencor/opencor/issues/786\">#786</a>)." },
                       { "change": "<strong>CellML Text view:</strong> added support for some trigonometric operators, <code>min(a, b, ...)</code> and <code>max(a, b, ...)</code> (see issue <a href=\"https://github.com/opencor/opencor/issues/789\">#789</a>), and <code>gcd(a, b, ...)</code> and <code>lcm(a, b, ...)</code> (see issue <a href=\"https://github.com/opencor/opencor/issues/790\">#790</a>)." },
                       { "change": "<strong>Single Cell view:</strong> create a new or update an existing CellML file based on the results of a simulation (see issue <a href=\"https://github.com/opencor/opencor/issues/33\">#33</a>). Made the previous feature optional (see issue <a href=\"https://github.com/opencor/opencor/issues/810\">#810</a>). Respect CellML 1.1 model modularity (see issue <a href=\"https://github.com/opencor/opencor/issues/568\">#568</a>). Only export the relevant simulation data from CellML 1.1 files (see issue <a href=\"https://github.com/opencor/opencor/issues/808\">#808</a>). The X axis now always line up when using multiple graph panels (see issue <a href=\"https://github.com/opencor/opencor/issues/615\">#615</a>)." },
                       { "change": "<strong>Single Cell view + SED-ML:</strong> improved support on Windows. Export a simulation to a COMBINE Archive file (see issue <a href=\"https://github.com/opencor/opencor/issues/743\">#743</a>). Namespace prefixes used in XPath expressions are now defined (see issue <a href=\"https://github.com/opencor/opencor/issues/775\">#775</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://www.llvm.org/\">LLVM</a> to version 3.7.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/739\">#739</a>), <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.23.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/780\">#780</a>) and <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.9.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/798\">#798</a>)." }
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
                       { "change": "<strong>Development:</strong> upgraded to <a href=\"https://developer.apple.com/xcode/\">Xcode</a> 7.0." },
                       { "change": "<strong>General:</strong> improved Internet-related features (to prevent the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a> from hanging up unnecessarily) and the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>'s integrity (during lengthy processes, e.g. fully instantiating a <a href=\"https://www.cellml.org/\">CellML</a> 1.1 model), and added <a href=\"https://en.wikipedia.org/wiki/Zip_(file_format)\">ZIP</a> support (for our <a href=\"https://github.com/opencor/opencor/issues/743\">ongoing work</a> on supporting the <a href=\"http://co.mbine.org/documents/archive\">COMBINE Archive format</a>)." },
                       { "change": "<strong>Viewer widget:</strong> improved the rendering of mathematical equations and made it possible (on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>) to copy a rendered equation to the clipboard." },
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
