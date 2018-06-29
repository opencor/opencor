var jsonData = { "versions": [
                   { "major": 0, "minor": 0, "patch": 0, "day": 27, "month": 6, "year": 2018, "type": 2,
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
                       { "change": "<strong>General:</strong> fixed a problem with not being able to start OpenCOR from the system PATH (see issue <a href=\"https://github.com/opencor/opencor/issues/1688\">#1688</a>). Fixed a regression issue with the wrong view being potentially shown when switching between files in different modes/views (see issue <a href=\"https://github.com/opencor/opencor/issues/1699\">#1699</a>). Moved our API plugins to the third-party category and, in some cases, renamed them (see issue <a href=\"https://github.com/opencor/opencor/issues/1707\">#1707</a>)." },
                       { "change": "<strong>Text-based editors:</strong> prevent infinite \"Replace all\" actions (see issue <a href=\"https://github.com/opencor/opencor/issues/1677\">#1677</a>). Sped up our highlight/replace all method (see issue <a href=\"https://github.com/opencor/opencor/issues/1679\">#1679</a>). Now have word-wrap alignment <a href=\"https://github.com/opencor/opencor/issues/1680\">#1680</a>)." },
                       { "change": "<strong>Simualtion Experiment view:</strong> fixed an issue with the Y axis being doubled in a very specific case (see issue <a href=\"https://github.com/opencor/opencor/issues/1683\">#1683</a>). Fixed some GUI glitches (see issue <a href=\"https://github.com/opencor/opencor/issues/1696\">#1696</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.27.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1672\">#1672</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.10.6 (see issue <a href=\"https://github.com/opencor/opencor/issues/1704\">#1704</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 29, "month": 5, "year": 2018, "type": 2,
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
                       { "change": "<strong>General:</strong> fixed an issue with version 55 of ICU being wrongly needed on some Linux systems (see issue <a href=\"https://github.com/opencor/opencor/issues/1627\">#1627</a>). Added support for exiting full-screen mode when pressing the Esc key on macOS (see issue <a href=\"https://github.com/opencor/opencor/issues/1637\">#1637</a>). Stopped using <a href=\"https://travis-ci.org/\">Travis CI</a> (since we now have everything set up and working nicely on <a href=\"http://autotest.bioeng.auckland.ac.nz/jenkins/view/OpenCOR/\">Jenkins</a>)." },
                       { "change": "<strong>SBML API:</strong> don't build libSBML with bzip2 or zip compression anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/1628\">#1628</a>)." },
                       { "change": "<strong>QScintilla support:</strong> now highlight the searched text, both in the editor and its vertical scroll bar (see issue <a href=\"https://github.com/opencor/opencor/issues/1414\">#1414</a>)." },
                       { "change": "<strong>Core plugin:</strong> fixed a problem with the menu items for recently opened files potentially appearing as disabled on macOS (see issue <a href=\"https://github.com/opencor/opencor/issues/1633\">#1633</a>)." },
                       { "change": "<strong>Text-based editors:</strong> can now word-wrap an editor's contents <a href=\"https://github.com/opencor/opencor/issues/1652\">#1652</a>)." },
                       { "change": "<strong>CellML Text view:</strong> keep track of a CellML file’s documentation (see issue <a href=\"https://github.com/opencor/opencor/issues/1642\">#1642</a>)." },
                       { "change": "<strong>Simualtion Experiment view:</strong> fixed a problem with a SED-ML file being considered modified under certain conditions (see issue <a href=\"https://github.com/opencor/opencor/issues/1626\">#1626</a>). Now keep track of unselected graphs in a SED-ML file / COMBINE archive (see issue <a href=\"https://github.com/opencor/opencor/issues/1631\">#1631</a>). Now provide a default value for the legend title of a valid graph (see issue <a href=\"https://github.com/opencor/opencor/issues/1662\">#1662</a>). Now automatically assign a colour to a new graph (see issue <a href=\"https://github.com/opencor/opencor/issues/1665\">#1665</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 6.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1552\">#1552</a>). Upgraded the <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 3.1.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1643\">#1643</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 23, "month": 4, "year": 2018, "type": 2,
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
                       { "change": "<strong>General:</strong> upgraded to Qt 5.9.5 LTS (see issue <a href=\"https://github.com/opencor/opencor/issues/1615\">#1615</a>). Fixed some seemingly random crashes (especially on Windows; see issue <a href=\"https://github.com/opencor/opencor/issues/1574\">#1574</a>)." },
                       { "change": "<strong>Data store interface:</strong> fixed a problem with the returned size of a data store being wrong in some cases (see issue <a href=\"https://github.com/opencor/opencor/issues/1579\">#1579</a>)." },
                       { "change": "<strong>Graph Panel widget:</strong> fixed a problem with graph panels not always being properly aligned (see issue <a href=\"https://github.com/opencor/opencor/issues/1619\">#1619</a>)." },
                       { "change": "<strong>PMR support:</strong> fixed a problem with the PMR Workspaces window potentially not reflecting the Git status of a folder (see issue <a href=\"https://github.com/opencor/opencor/issues/1582\">#1582</a>). Updated the description of our different PMR instances (see issue <a href=\"https://github.com/opencor/opencor/issues/1583\">#1583</a>)." },
                       { "change": "<strong>CellML support:</strong> now provide more information when imports cannot be fully instantiated (see issue <a href=\"https://github.com/opencor/opencor/issues/1595\">#1595</a>)." },
                       { "change": "<strong>SED-ML support:</strong> fixed a problem with OpenCOR crashing when saving an empty SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/1572\">#1572</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> fixed a problem that resulted in the the window being empty until reauthentication (see issue <a href=\"https://github.com/opencor/opencor/issues/1581\">#1581</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> prevent a model with no ODEs/DAEs from crashing OpenCOR (see issue <a href=\"https://github.com/opencor/opencor/issues/1576\">#1576</a>). Fixed an issue with some results being potentially not plotted at all (see issue <a href=\"https://github.com/opencor/opencor/issues/1613\">#1613</a>). Fixed a problem with legend labels not always being right (see issue <a href=\"https://github.com/opencor/opencor/issues/1620\">#1620</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> have libgit2 use our copy of zlib rather than the one provided by the system (see issue <a href=\"https://github.com/opencor/opencor/issues/1577\">#1577</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.27.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1588\">#1588</a>). Upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2o (see issue <a href=\"https://github.com/opencor/opencor/issues/1589\">#1589</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.10.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1608\">#1608</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 17, "month": 3, "year": 2018, "type": 2,
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
                       { "change": "<strong>PMR support:</strong> now provide better error messages (see issue <a href=\"https://github.com/opencor/opencor/issues/1556\">#1556</a>). Now provide a more meaningful error message when something goes wrong (see issue <a href=\"https://github.com/opencor/opencor/issues/1558\">#1558</a>). Fixed a problem with folders being potentially incorrectly tracked (see issue <a href=\"https://github.com/opencor/opencor/issues/1559\">#1559</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> improved the synchronisation with PMR in case the Internet connection drops (see issue <a href=\"https://github.com/opencor/opencor/issues/1566\">#1566</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> fixed a small regression problem with nothing being shown when running a simulation and then asking for a graph to be plotted (see issue <a href=\"https://github.com/opencor/opencor/issues/1560\">#1560</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.26.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/1557\">#1557</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 9, "month": 3, "year": 2018, "type": 2,
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
                       { "change": "<strong>Simulation Experiment view:</strong> prevent the legend labels from disappearing upon clearing the simulation data (see issue <a href=\"https://github.com/opencor/opencor/issues/1537\">#1537</a>). Prevent the clearing of simulation data from taking too long when we have several graph panels (see issue <a href=\"https://github.com/opencor/opencor/issues/1538\">#1538</a>). Fixed a problem that prevented, on Windows, a simulation from being exported to a COMBINE archive (see issue <a href=\"https://github.com/opencor/opencor/issues/1553\">#1553</a>)." },
                       { "change": "<strong>Simulation support:</strong> a change in a model parameter now results in a solver being reinitialised (see issue <a href=\"https://github.com/opencor/opencor/issues/1549\">#1549</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.10.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/1548\">#1548</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 1, "month": 2, "year": 2018, "type": 2,
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
                       { "change": "<strong>Simulation Experiment view:</strong> prevent the legend labels from disappearing upon clearing the simulation data (see issue <a href=\"https://github.com/opencor/opencor/issues/1537\">#1537</a>). Prevent the clearing of simulation data from taking too long when we have several graph panels (see issue <a href=\"https://github.com/opencor/opencor/issues/1538\">#1538</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 31, "month": 1, "year": 2018, "type": 2,
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
                       { "change": "<strong>General:</strong> upgraded to Qt 5.9.4 LTS (see issue <a href=\"https://github.com/opencor/opencor/issues/1526\">#1526</a>). A new file (with no specific type) is not wrongly considered to be a COMBINE archive anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/1530\">#1530</a>)." },
                       { "change": "<strong>PMR window:</strong> properly refresh the list of public exposures when changing the PMR instance (see issue <a href=\"https://github.com/opencor/opencor/issues/1521\">#1521</a>)." },
                       { "change": "<strong>Simulation support:</strong> addressed a problem with fixing and then saving a previously invalid simulation (see issue <a href=\"https://github.com/opencor/opencor/issues/1533\">#1533</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> added support for multiple runs (see issue <a href=\"https://github.com/opencor/opencor/issues/1523\">#1523</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 12, "month": 1, "year": 2018, "type": 2,
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
                       { "change": "<strong>General:</strong> improved our support for <a href=\"https://en.wikipedia.org/wiki/Differential_algebraic_equation\">DAE</a> models (see issues <a href=\"https://github.com/opencor/opencor/issues/188\">#188</a> and <a href=\"https://github.com/opencor/opencor/issues/536\">#536</a>)." },
                       { "change": "<strong>Solvers:</strong> can now parameterise the <a href=\"https://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> solver (see issue <a href=\"https://github.com/opencor/opencor/issues/1510\">#1510</a>). Don't reset the <a href=\"https://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> solver on consecutive runs and improved its implementation (see issue <a href=\"https://github.com/opencor/opencor/issues/1512\">#1512</a>). Removed the <a href=\"https://computation.llnl.gov/projects/sundials/idas\">IDAS</a> solver (see issue <a href=\"https://github.com/opencor/opencor/issues/1515\">#1515</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> don’t incorrectly mark a CellML file as modified (see issue <a href=\"https://github.com/opencor/opencor/issues/1513\">#1513</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded the <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 3.1.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1507\">#1507</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 23, "month": 12, "year": 2017, "type": 2,
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
                       { "change": "<strong>General:</strong> addressed a problem with quickly changing files potentially resulting in the wrong view being shown (see issue <a href=\"https://github.com/opencor/opencor/issues/1481\">#1481</a>). Reworked our approach to the saving/reloading of a file (see issue <a href=\"https://github.com/opencor/opencor/issues/1491\">#1491</a>). Now pass `make[test].bat` arguments to `scripts/genericmake.bat` (see issue <a href=\"https://github.com/opencor/opencor/issues/1498\">#1498</a>)." },
                       { "change": "<strong>CellML API:</strong> fixed a small problem that made OpenCOR crash if a metadata id contained \":\" (see issue <a href=\"https://github.com/opencor/opencor/issues/1487\">#1487</a>)." },
                       { "change": "<strong>Graph panel widget:</strong> have the legend buttons use the same background as the plot area (see issue <a href=\"https://github.com/opencor/opencor/issues/1477\">#1477</a>). Got the legend context menu to work again (see issue <a href=\"https://github.com/opencor/opencor/issues/1478\">#1478</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> now allow the editing and saving of a SED-ML file / COMBINE archive (see issue <a href=\"https://github.com/opencor/opencor/issues/1366\">#1366</a>). To reload a CellML file when in development mode now actually reloads it (see issue <a href=\"https://github.com/opencor/opencor/issues/1488\">#1488</a>). Made the (re)loading of remote files and SED-ML files / COMBINE archives much smoother (see issue <a href=\"https://github.com/opencor/opencor/issues/1492\">#1492</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2n (see issue <a href=\"https://github.com/opencor/opencor/issues/1483\">#1483</a>). Upgraded <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a> to version 5.16.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1494\">#1494</a>). Upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 5.0.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1502\">#1502</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 29, "month": 11, "year": 2017, "type": 2,
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
                       { "change": "<strong>Graph panel widget:</strong> now prevent the plot canvas can from becoming black in some cases (see issue <a href=\"https://github.com/opencor/opencor/issues/1475\">#1475</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> can now distinguish between the background colour of the plot area and that of the surrounding area (see issue <a href=\"https://github.com/opencor/opencor/issues/1473\">#1473</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 28, "month": 11, "year": 2017, "type": 2,
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
                       { "change": "<strong>General:</strong> upgraded to Qt 5.9.3 LTS (see issue <a href=\"https://github.com/opencor/opencor/issues/1464\">#1464</a>)." },
                       { "change": "<strong>Graph panel widget:</strong> axis labels are now rendered fine when exporting a graph panel to various formats (see issue <a href=\"https://github.com/opencor/opencor/issues/1448\">#1448</a>). Can now interact with a graph panel after it has been exported to a given format (see issue <a href=\"https://github.com/opencor/opencor/issues/1450\">#1450</a>)." },
                       { "change": "<strong>SED-ML support:</strong> now track the relative height of a graph panel (see issue <a href=\"https://github.com/opencor/opencor/issues/1461\">#1461</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> it is now possible to customise a graph panel and have it serialised to / retrieved from a SED-ML file (see issue <a href=\"https://github.com/opencor/opencor/issues/1426\">#1426</a>). Can now show the legend of the different graphs shown in a graph panel (see issue <a href=\"https://github.com/opencor/opencor/issues/1171\">#1171</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded the <a href=\"http://computation.llnl.gov/projects/sundials\">SUNDIALS</a> library to version 3.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1453\">#1453</a>). Upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2m (see issue <a href=\"https://github.com/opencor/opencor/issues/1458\">#1458</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.10.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/1466\">#1466</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 23, "month": 10, "year": 2017, "type": 2,
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
                       { "change": "<strong>General:</strong> upgraded to Qt 5.9.2 LTS (see issue <a href=\"https://github.com/opencor/opencor/issues/1311\">#1311</a>). Switched to QtWebKit Reloaded (see issue <a href=\"https://github.com/opencor/opencor/issues/1149\">#1149</a>)." },
                       { "change": "<strong>Plugins dialog:</strong> make the value of the different fields selectable (see issue <a href=\"https://github.com/opencor/opencor/issues/1444\">#1444</a>)." },
                       { "change": "<strong>PMR window:</strong> retrieve public exposures only when really needed." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 5, "month": 10, "year": 2017, "type": 2,
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
                       { "change": "<strong>APIs:</strong> have HAVE_IEEEFP_H properly set in the libSBML, libNuML and libSEDML packages under Windows (see issue <a href=\"https://github.com/opencor/opencor/issues/1432\">#1432</a>). Upgraded the libSEDML package to version 0.4.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/1433\">#1433</a>)." },
                       { "change": "<strong>Graph panel widget:</strong> can now export a graph panel to various formats (e.g. PDF, PNG, SVG) (see issue <a href=\"https://github.com/opencor/opencor/issues/1123\">#1123</a>). Greatly improved our support for log scales (see issue <a href=\"https://github.com/opencor/opencor/issues/1412\">#1412</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> added support for the alpha channel in the colours used to customise a graph. Made sure that the GUI is properly initialised when (re)loading a SED-ML file." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 25, "month": 9, "year": 2017, "type": 2,
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
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>ZIP support:</strong> use 755 for directories and 644 for files, if no file permissions are set (see issue <a href=\"https://github.com/opencor/opencor/issues/1417\">#1417</a>)." },
                       { "change": "<strong>Text-based editors:</strong> added line numbers (see issue <a href=\"https://github.com/opencor/opencor/issues/1413\">#1413</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> added the ability to customise a graph (see issue <a href=\"https://github.com/opencor/opencor/issues/591\">#591</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 13, "month": 9, "year": 2017, "type": 2,
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
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> use <a href=\"https://www.docker.com/\">Docker</a> with <a href=\"https://en.wikipedia.org/wiki/Ubuntu_(operating_system)\">Ubuntu</a> 16.04 LTS on <a href=\"https://travis-ci.org/\">Travis CI</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/1395\">#1395</a>)." },
                       { "change": "<strong>Graph panel widget:</strong> fixed and improved the synchronisation of axes. Added support for log scales (see issue <a href=\"https://github.com/opencor/opencor/issues/1299\">#1299</a>)." },
                       { "change": "<strong>Solvers:</strong> reference <a href=\"https://computation.llnl.gov/projects/sundials/cvodes\">CVODES</a>/<a href=\"https://computation.llnl.gov/projects/sundials/idas\">IDAS</a> rather than <a href=\"https://computation.llnl.gov/projects/sundials/cvode\">CVODE</a>/<a href=\"https://computation.llnl.gov/projects/sundials/ida\">IDA</a> (see issue <a href=\"https://github.com/opencor/opencor/issues/1405\">#1405</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://khan.github.io/KaTeX/\">KaTeX</a> to version 0.8.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/1401\">#1401</a>). Upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 5.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1407\">#1407</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 12, "month": 8, "year": 2017, "type": 2,
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
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> make sure that we can find the plugin 'xcb' on some Linux machines (see issue <a href=\"https://github.com/opencor/opencor/issues/1383\">#1383</a>)." },
                       { "change": "<strong>CSV data store:</strong> make sure that we can export to a (really) big CSV file (see issue <a href=\"https://github.com/opencor/opencor/issues/1394\">#1394</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> make sure that the runtime gets updated after having edited a CellML file (see issue <a href=\"https://github.com/opencor/opencor/issues/1385\">#1385</a>). Don't check whether the memory associated with a simulation can be allocated (see issue <a href=\"https://github.com/opencor/opencor/issues/1389\">#1389</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://github.com/OpenCMISS/zinc\">Zinc</a> to version 3.1.2 (see issue <a href=\"https://github.com/opencor/opencor/issues/1393\">#1393</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 23, "month": 7, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> added a simulation support plugin (to separate out running of a simulation from its widget) (see issue <a href=\"https://github.com/opencor/opencor/issues/1266\">#1266</a>). Provide an error message when a plugin is considered to be invalid (see issue <a href=\"https://github.com/opencor/opencor/issues/1347\">#1347</a>). Make certain interfaces available to other plugins (see issue <a href=\"https://github.com/opencor/opencor/issues/1371\">#1371</a>)." },
                       { "change": "<strong>Text-based editors:</strong> don't reset the font size anymore when reloading a file (see issue <a href=\"https://github.com/opencor/opencor/issues/1369\">#1369</a>)." },
                       { "change": "<strong>Property Editor widget:</strong> disallow navigation using the (Shift+)Tab key(s) (see issue <a href=\"https://github.com/opencor/opencor/issues/1354\">#1354</a>)." },
                       { "change": "<strong>SUNDIALS package:</strong> don't package <a href=\"http://computation.llnl.gov/projects/sundials/cvode\">CVODE</a> and <a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> anymore (to avoid conflicts with <a href=\"http://computation.llnl.gov/projects/sundials/cvodes\">CVODES</a> and <a href=\"http://computation.llnl.gov/projects/sundials/idas\">IDAS</a>; see issue <a href=\"https://github.com/opencor/opencor/issues/1367\">#1367</a>)." },
                       { "change": "<strong>Simulation Experiment view:</strong> prevent the renaming of a file from messing up a locked graph (see issue <a href=\"https://github.com/opencor/opencor/issues/1365\">#1365</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.26 (see issue <a href=\"https://github.com/opencor/opencor/issues/1340\">#1340</a>). Upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 4.0.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1352\">#1352</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.10.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1358\">#1358</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 21, "month": 6, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> our API and third-party libraries are now taking advantage of our new packaging mechanism (see issue <a href=\"https://github.com/opencor/opencor/issues/1306\">#1306</a>). OpenGL now works in an Ubuntu virtual machine (see issue <a href=\"https://github.com/opencor/opencor/issues/1307\">#1307</a>). Added OpenSSL as a third-party plugin (see issues <a href=\"https://github.com/opencor/opencor/issues/1101\">#1101</a> and <a href=\"https://github.com/opencor/opencor/issues/1314\">#1314</a>). Can now open remote binary files (see issue <a href=\"https://github.com/opencor/opencor/issues/1319\">#1319</a>)." },
                       { "change": "<strong>Graph panel widget:</strong> improved the plotting of small values (see issue <a href=\"https://github.com/opencor/opencor/issues/1322\">#1322</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2l (see issue <a href=\"https://github.com/opencor/opencor/issues/1318\">#1318</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 7, "month": 5, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> now provide a mechanism to package and download pre-built libraries (see issue <a href=\"https://github.com/opencor/opencor/issues/1106\">#1106</a>). Can now download an official version of OpenCOR from either the OpenCOR website or GitHub (see issue <a href=\"https://github.com/opencor/opencor/issues/1154\">#1154</a>). Changed our license to GPL v3.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1301\">#1301</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 28, "month": 4, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>Build system:</strong> make sure that the dependencies of external binaries are properly referenced <a href=\"https://github.com/opencor/opencor/issues/1281\">#1281</a>)." },
                       { "change": "<strong>General:</strong> Added support for the <a href=\"https://github.com/OpenCMISS/zinc\">Zinc</a> library (see issue <a href=\"https://github.com/opencor/opencor/issues/15\">#15</a>). Don't make our windows have a minimum size (see issue <a href=\"https://github.com/opencor/opencor/issues/1260\">#1260</a>)." },
                       { "change": "<strong>CLI:</strong> make it possible to include/exclude a plugin to/from the GUI version of OpenCOR (see issue <a href=\"https://github.com/opencor/opencor/issues/1292\">#1292</a>)." },
                       { "change": "<strong>File Handling interface:</strong> removed isIndirectRemoteFile() since it's not being used anymore (see issue <a href=\"https://github.com/opencor/opencor/issues/1267\">#1267</a>)." },
                       { "change": "<strong>CellML API:</strong> fixed a problem that made it crash OpenCOR when defining a certain type of unit (see issue <a href=\"https://github.com/opencor/opencor/issues/1278\">#1278</a>)." },
                       { "change": "<strong>Property Editor widget:</strong> allow for navigation using the (Shift+)Tab key(s) (see issue <a href=\"https://github.com/opencor/opencor/issues/1273\">#1273</a>). Don't go to the next property when cancelling the editing of a property (direct consequence of our work on issue <a href=\"https://github.com/opencor/opencor/issues/1273\">#1273</a>; see issue <a href=\"https://github.com/opencor/opencor/issues/1285\">#1285</a>)." },
                       { "change": "<strong>PMR-based windows:</strong> now clear the contents of those windows before switching instances (see issue <a href=\"https://github.com/opencor/opencor/issues/1279\">#1279</a>)." },
                       { "change": "<strong>PMR window:</strong> make sure that the busy widget always disappears (see issue <a href=\"https://github.com/opencor/opencor/issues/1288\">#1288</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> provide more succinct diff results (see issue <a href=\"https://github.com/opencor/opencor/issues/1294\">#1294</a>)." },
                       { "change": "<strong>CellML Text view:</strong> the import of a CellML file is now always properly converted (see issue <a href=\"https://github.com/opencor/opencor/issues/1257\">#1257</a>)." },
                       { "change": "<strong>Single Cell view:</strong> renamed the view to Simulation Experiment view (see issue <a href=\"https://github.com/opencor/opencor/issues/1256\">#1256</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 3.2.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1264\">#1264</a>)" }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 24, "month": 3, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>CellML Text view:</strong> properly process unknown elements (see issue <a href=\"https://github.com/opencor/opencor/issues/1253\">#1253</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://jquery.com/\">jQuery</a> to version 3.2.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1252\">#1252</a>). Upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 4.0.0 (see issue <a href=\"https://github.com/opencor/opencor/issues/1248\">#1248</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 15, "month": 3, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> added some libraries to the Linux package for OpenCOR to work on Fedora (see issue <a href=\"https://github.com/opencor/opencor/issues/1244\">#1244</a>)." },
                       { "change": "<strong>PMR window:</strong> prevented exposure files from being retrieved (and listed) twice (see issue <a href=\"https://github.com/opencor/opencor/issues/1243\">#1243</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> now propagate the state of a workspace in the icon used for a folder (see issue <a href=\"https://github.com/opencor/opencor/issues/1246\">#1246</a>)." },
                       { "change": "<strong>CellML Text view:</strong> now ignore `documentation` elements rather than consider them as unknown (and therefore 'fail'; see issue <a href=\"https://github.com/opencor/opencor/issues/1245\">#1245</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 4, "month": 3, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>PMR Workspaces window:</strong> fixed a crash that used to occur when expanding a workspace by clicking on its arrow (\">\") when nothing else was selected (see issue <a href=\"https://github.com/opencor/opencor/issues/1242\">#1242</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 3, "month": 3, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> fixed a small problem with the desktop icon not showing on Ubuntu (see issue <a href=\"https://github.com/opencor/opencor/issues/1235\">#1235</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> fixed a small problem with synchronising potentially resulting in a segmentation fault on Linux (see issue <a href=\"https://github.com/opencor/opencor/issues/1236\">#1236</a>). Can now create a workspace from a non-empty (and non-Git) folder (see issue <a href=\"https://github.com/opencor/opencor/issues/1237\">#1237</a>). Now make it obvious which PMR site is being used (see issue <a href=\"https://github.com/opencor/opencor/issues/1239\">#1239</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 24, "month": 2, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> keep track of the position and size of dialogs (see issue <a href=\"https://github.com/opencor/opencor/issues/1224\">#1224</a>)." },
                       { "change": "<strong>PMR support:</strong> let the user know if we simply cannot authenticate (see issue <a href=\"https://github.com/opencor/opencor/issues/1223\">#1223</a>)." },
                       { "change": "<strong>PMR-based windows:</strong> only reload them (upon changes to preferences) if the PMR URL is different (see issue <a href=\"https://github.com/opencor/opencor/issues/1222\">#1222</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> got rid of the stage/unstage context menu item (for a file; see issue <a href=\"https://github.com/opencor/opencor/issues/1212\">#1212</a>). Use the same context menu for the workspace, folders and files (see issue <a href=\"https://github.com/opencor/opencor/issues/1213\">#1213</a>). Offer a way to see the differences in the files that are to be synchronised with PMR (see issue <a href=\"https://github.com/opencor/opencor/issues/1229\">#1229</a>)." },
                       { "change": "<strong>CellML Text view:</strong> unknown elements (e.g. `int`) now result in an error during conversion to the CellML Text format (see issue <a href=\"https://github.com/opencor/opencor/issues/1230\">#1230</a>). Fixed a small problem with trying to comment out an empty line moving the caret to the next line (see issue <a href=\"https://github.com/opencor/opencor/issues/1232\">#1232</a>)." },
                       { "change": "<strong>WebViewer widget based plugins:</strong> fixed a small ‘regression’ issue with the default context menu always being shown (see issue <a href=\"https://github.com/opencor/opencor/issues/1233\">#1233</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.10 (see issue <a href=\"https://github.com/opencor/opencor/issues/1227\">#1227</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 10, "month": 2, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> various minor GUI improvements here and there." },
                       { "change": "<strong>WebViewer widget:</strong> allow the use of a progress bar (see issue <a href=\"https://github.com/opencor/opencor/issues/1219\">#1219</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> added the 'teaching' instance to the list of PMR instances (see issue <a href=\"https://github.com/opencor/opencor/issues/1218\">#1218</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 8, "month": 2, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>CellML support:</strong> generate an error when a model has more than one variable of integration (see issue <a href=\"https://github.com/opencor/opencor/issues/1210\">#1210</a>)." },
                       { "change": "<strong>PMR Workspaces window:</strong> allow the user's name and email to be set in Preferences (see issue <a href=\"https://github.com/opencor/opencor/issues/1057\">#1057</a>). Allow the PMR instance to be set in Preferences (see issue <a href=\"https://github.com/opencor/opencor/issues/1069\">#1069</a>). Fixed a problem with the log on/off icon not showing properly on a Mac with a HiDPI screen resolution (see issue <a href=\"https://github.com/opencor/opencor/issues/1204\">#1204</a>). Let the user know if no name and/or email are provided (see issue <a href=\"https://github.com/opencor/opencor/issues/1208\">#1208</a>). Warn the user if we try to commit without a message (see issue <a href=\"https://github.com/opencor/opencor/issues/1209\">#1209</a>). Don't rely on an external browser to do the PMR authentication (see issue <a href=\"https://github.com/opencor/opencor/issues/1211\">#1211</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 1, "month": 2, "year": 2017, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> simplified the build process on macOS when linking libraries (see issue <a href=\"https://github.com/opencor/opencor/issues/1159\">#1159</a>). Prevented old plugins from potentially crashing OpenCOR (see issue <a href=\"https://github.com/opencor/opencor/issues/1163\">#1163</a>). Converted the README files to reStructuredText (see issue <a href=\"https://github.com/opencor/opencor/issues/1170\">#1170</a>)." },
                       { "change": "<strong>CLI:</strong> options can now be passed to plugins (see issue <a href=\"https://github.com/opencor/opencor/issues/1166\">#1166</a>). Fixed a misleading help message when the command specifies a missing plugin (see issue <a href=\"https://github.com/opencor/opencor/issues/1168\">#1168</a>). A plugin's initialisation and finalisation code now gets called when run from the CLI (see issue <a href=\"https://github.com/opencor/opencor/issues/1173\">#1173</a>)." },
                       { "change": "<strong>Organisation:</strong> improved the cloning of a workspace from the PMR window (see issue <a href=\"https://github.com/opencor/opencor/issues/848\">#848</a>). Added a new PMR Workspaces window (see issues <a href=\"https://github.com/opencor/opencor/issues/969\">#969</a>, <a href=\"https://github.com/opencor/opencor/issues/1124\">#1124</a> and <a href=\"https://github.com/opencor/opencor/issues/1141\">#1141</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://khan.github.io/KaTeX/\">KaTeX</a> to version 0.7.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1195\">#1195</a>). Upgraded <a href=\"http://www.llvm.org/\">LLVM</a>+<a href=\"http://clang.llvm.org/\">Clang</a> to version 3.9.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1099\">#1099</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.25.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1190\">#1190</a>). Upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2k (see issue <a href=\"https://github.com/opencor/opencor/issues/1202\">#1202</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.9.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1179\">#1179</a>). Upgraded <a href=\"http://www.zlib.net/\">zlib</a> to version 1.2.11 (see issue <a href=\"https://github.com/opencor/opencor/issues/1193\">#1193</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 17, "month": 11, "year": 2016, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> fixed a small problem with the desktop icon not showing on Ubuntu (see issue <a href=\"https://github.com/opencor/opencor/issues/1147\">#1147</a>). Now deploy the Universal C Runtime files needed by OpenCOR (see issue <a href=\"https://github.com/opencor/opencor/issues/1150\">#1150</a>)." },
                       { "change": "<strong>Web Browser window:</strong> made sure that all Web Inspector windows are closed upon quitting OpenCOR (see issue <a href=\"https://github.com/opencor/opencor/issues/1151\">#1151</a>). Added support the OpenCOR URL scheme (see issue <a href=\"https://github.com/opencor/opencor/issues/1152\">#1152</a>)." },
                       { "change": "<strong>Raw Text view:</strong> fixed a crash that occurred when trying to save a new file (see issue <a href=\"https://github.com/opencor/opencor/issues/1155\">#1155</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.24.3 (see issue <a href=\"https://github.com/opencor/opencor/issues/1145\">#1145</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 0, "patch": 0, "day": 9, "month": 11, "year": 2016, "type": 2,
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
                     ],
                     "changes": [
                       { "change": "<strong>General:</strong> improved the translation of Qt-related messages (see issue <a href=\"https://github.com/opencor/opencor/issues/1135\">#1135</a>). Improved support for macOS (see issues <a href=\"https://github.com/opencor/opencor/issues/1134\">#1134</a> and <a href=\"https://github.com/opencor/opencor/issues/1137\">#1137</a>). Now have a Preferences dialog so the user can specify his preferences for OpenCOR and its plugins (see issue <a href=\"https://github.com/opencor/opencor/issues/193\">#193</a>)." },
                       { "change": "<strong>Data store:</strong> made it possible to select a group of data (see issue <a href=\"https://github.com/opencor/opencor/issues/1143\">#1143</a>)." },
                       { "change": "<strong>Web Browser window:</strong> added a Web browser like window (see issue <a href=\"https://github.com/opencor/opencor/issues/1115\">#1115</a>)." }
                     ]
                   }
                 ]
               };
