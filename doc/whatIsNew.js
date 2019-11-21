var jsonData = { "versions": [
                   { "anchor": "latest", "description": "Latest snapshot", "day": 9, "month": 9, "year": 2019,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                           { "type": "added", "description": "Support for <a href=\"https://www.python.org/\">Python</a>." }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "fixed", "description": "Crash when a file has been rendered invalid." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "improved", "description": "Unicode support." }
                             ]
                           }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "fixed", "description": "Reading a <a href=\"https://www.cellml.org/\">CellML</a> file that has no <a href=\"https://www.w3.org/XML/\">XML</a> processing instruction." }
                             ]
                           }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "PMR Workspaces window",
                             "entries": [
                               { "type": "improved", "description": "Synchronisation dialog." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Simulation Experiment view",
                             "entries": [
                               { "type": "added", "description": "Basic support for the import of external data." },
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "improved", "description": "Plotting of simulations." },
                               { "type": "fixed", "description": "Issue with handling the Graph Panel properties." },
                               { "type": "fixed", "description": "Various issues with saving a file under a new name." },
                               { "type": "fixed", "description": "Issue with a <a href=\"https://sed-ml.github.io/\">SED-ML</a> file / <a href=\"https://co.mbine.org/documents/archive\">COMBINE archive</a> not necessarily saving all of a solver's properties." },
                               { "type": "fixed", "description": "Issue with a <a href=\"https://sed-ml.github.io/\">SED-ML</a> file / <a href=\"https://co.mbine.org/documents/archive\">COMBINE archive</a> being considered modified after changing languages." }
                             ]
                           }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "subCategory", "name": "COMBINE support",
                             "entries": [
                               { "type": "fixed", "description": "Files with a master attribute of \"1\" (rather than \"true\")." }
                             ]
                           },
                           { "type": "subCategory", "name": "<a href=\"https://sed-ml.github.io/\">SED-ML</a> support",
                             "entries": [
                               { "type": "added", "description": "Support for left and right triangle symbols." }
                             ]
                           },
                           { "type": "subCategory", "name": "Simulation support",
                             "entries": [
                               { "type": "improved", "description": "Accuracy of algebraic variables that depend on the value of some state variables." },
                               { "type": "fixed", "description": "Issue with a remote <a href=\"https://sed-ml.github.io/\">SED-ML</a> file being run headless." }
                             ]
                           }
                         ]
                       },
                       { "name": "Tools",
                         "entries": [
                           { "type": "subCategory", "name": "CellML tools",
                             "entries": [
                               { "type": "added", "description": "Validation of a <a href=\"https://www.cellml.org/\">CellML</a> file from the <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a>." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.6", "description": "Version 0.6", "day": 11, "month": 12, "year": 2018,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "added", "description": "Preferences dialog." },
                           { "type": "added", "description": "include/exclude plugins from the <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a>." },
                           { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Linux\">Linux</a>." },
                           { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/MacOS\">macOS</a>." },
                           { "type": "improved", "description": "Plugins dialog." },
                           { "type": "improved", "description": "Plugin framework." },
                           { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                           { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Differential_algebraic_equation\">DAE</a> models." },
                           { "type": "improved", "description": "Keyboard navigation in tree views." }
                         ]
                       },
                       { "name": "API",
                         "entries": [
                           { "type": "added", "description": "<a href=\"https://github.com/NuML/NuML/\">NuML</a> API (via <a href=\"https://github.com/NuML/NuML/\">libNuML</a>)." }
                         ]
                       },
                       { "name": "Data store",
                         "entries": [
                           { "type": "improved", "description": "Selection of data." },
                           { "type": "subCategory", "name": "<a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> data store",
                             "entries": [
                               { "type": "improved", "description": "Export of data." }
                             ]
                           }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "Text-based view",
                             "entries": [
                               { "type": "added", "description": "Word wrap." },
                               { "type": "added", "description": "Line numbers." },
                               { "type": "added", "description": "Text highlighting." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "improved", "description": "Conversion of <a href=\"https://www.cellml.org/\">CellML</a> files to the <a href=\"user/plugins/editing/cellmlTextView.html#plugins-editing-cellmltextview-cellmltextformat\">CellML Text format</a>." },
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." }
                             ]
                           }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "added", "description": "Web Browser window." }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "added", "description": "<a href=\"user/plugins/organisation/pmrWorkspacesWindow.html\">PMR Workspaces window</a>." },
                           { "type": "subCategory", "name": "PMR window",
                             "entries": [
                               { "type": "improved", "description": "Initialisation." },
                               { "type": "improved", "description": "Cloning of a workspace." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Simulation Experiment view (formerly known as the Single Cell view)",
                             "entries": [
                               { "type": "added", "description": "Support for multiple runs." },
                               { "type": "added", "description": "Ability to customise a graph." },
                               { "type": "added", "description": "Support for logarithmic scales." },
                               { "type": "added", "description": "Ability to customise a graph panel." },
                               { "type": "added", "description": "Ability to show the legend of a graph panel." },
                               { "type": "added", "description": "Support for pinch gestures to zoom in/out a graph panel." },
                               { "type": "added", "description": "Export a graph panel to various formats (e.g. PDF, PNG, SVG)." },
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "improved", "description": "Plotting of simulations." },
                               { "type": "improved", "description": "Colour for a new graph." },
                               { "type": "improved", "description": "Simulation time information." },
                               { "type": "improved", "description": "Running of long simulations." },
                               { "type": "improved", "description": "Resetting of model parameters." },
                               { "type": "improved", "description": "Synchronisation of the X and/or Y axes of all the graph panels." },
                               { "type": "fixed", "description": "<a href=\"https://computation.llnl.gov/projects/sundials/cvode\">CVODE</a> solver issue when modifying a model state." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "subCategory", "name": "<a href=\"https://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a> solver",
                             "entries": [
                               { "type": "added", "description": "Parametrisation of the solver." }
                             ]
                           },
                           { "type": "comment", "description": "The <a href=\"https://computation.llnl.gov/projects/sundials/ida\">IDA</a> solver was removed (in favour of <a href=\"https://computation.llnl.gov/projects/sundials/cvode\">CVODE</a>+<a href=\"https://computation.llnl.gov/projects/sundials/kinsol\">KINSOL</a>)." }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "subCategory", "name": "CellML support",
                             "entries": [
                               { "type": "improved", "description": "Generation of error messages." }
                             ]
                           },
                           { "type": "subCategory", "name": "<a href=\"https://sed-ml.github.io/\">SED-ML</a> support",
                             "entries": [
                               { "type": "added", "description": "Customisation of graph panels." }
                             ]
                           },
                           { "type": "subCategory", "name": "ZIP support",
                             "entries": [
                               { "type": "improved", "description": "decompression." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.5", "description": "Version 0.5", "day": 15, "month": 10, "year": 2016,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "added", "description": "Resetting all your settings from the <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a>." },
                           { "type": "added", "description": "Detection of changes to one or several file dependencies." },
                           { "type": "added", "description": "Support for an OpenCOR-specific URL scheme." },
                           { "type": "added", "description": "Default view to open a particular type of file." },
                           { "type": "added", "description": "Reference to the <a href=\"https://tutorial-on-cellml-opencor-and-pmr.readthedocs.io/en/latest/\">official OpenCOR tutorial</a>." },
                           { "type": "added", "description": "Use of a user's proxy settings." },
                           { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "improved", "description": "Handling of files." },
                           { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                           { "type": "improved", "description": "Rendering of equations." },
                           { "type": "improved", "description": "Editing of text-based files." },
                           { "type": "improved", "description": "Retrieval of a Save File name." },
                           { "type": "improved", "description": "Reopening of the most recent file." },
                           { "type": "fixed", "description": "Rendering of equations." },
                           { "type": "fixed", "description": "Copying of a rendered equation to the clipboard (on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>)." },
                           { "type": "fixed", "description": "Compiler engine crash due to an unfound external function (on <a href=\"https://en.wikipedia.org/wiki/Linux\">Linux</a>)." }
                         ]
                       },
                       { "name": "API",
                         "entries": [
                           { "type": "added", "description": "<a href=\"http://sbml.org/\">SBML</a> API (via <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a>)." },
                           { "type": "added", "description": "<a href=\"https://sed-ml.github.io/\">SED-ML</a> API (via <a href=\"https://github.com/fbergmann/libSEDML/\">libSEDML</a>)." }
                         ]
                       },
                       { "name": "Data store",
                         "entries": [
                           { "type": "added", "description": "BioSignalML data store." },
                           { "type": "subCategory", "name": "<a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> data store",
                             "entries": [
                               { "type": "added", "description": "Selection of model parameters prior to data export." },
                               { "type": "improved", "description": "Export of data." }
                             ]
                           }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw Text view (formerly known as the Raw view)",
                             "entries": [
                               { "type": "improved", "description": "Support for text files." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw CellML view",
                             "entries": [
                               { "type": "improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "fixed", "description": "Reformatting unescaped HTML entities." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "added", "description": "Import/export of the <a href=\"user/plugins/editing/cellmlTextView.html#plugins-editing-cellmltextview-cellmltextformat\">CellML Text format</a> from/to <a href=\"https://www.cellml.org/\">CellML</a>." },
                               { "type": "added", "description": "Support for the <code>min()</code>/<code>max()</code> and <code>gcd()</code>/<code>lcm()</code> functions." },
                               { "type": "added", "description": "Offer to save the contents of the view to a text file if it cannot be saved as a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "improved", "description": "Syntax highlighting." },
                               { "type": "improved", "description": "Support for comments." },
                               { "type": "improved", "description": "Support for <code>cmeta:id</code>'s." },
                               { "type": "improved", "description": "Saving of a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "fixed", "description": "Serialisation of a mathematical equation with a <code>cmeta:id</code> value." },
                               { "type": "fixed", "description": "Serialisation of numbers that have an exponent." },
                               { "type": "fixed", "description": "Serialisation of metadata." }
                             ]
                           },
                           { "type": "added", "description": "<a href=\"user/plugins/editing/rawSedmlView.html\">Raw SED-ML view</a>." }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "Physiome Model Repository window (formerly known as the CellML Model Repository window)",
                             "entries": [
                               { "type": "improved", "description": "Listing of exposures and exposure files." },
                               { "type": "added", "description": "Listing of <a href=\"https://sed-ml.github.io/\">SED-ML</a> files." },
                               { "type": "added", "description": "Cloning of a workspace." },
                               { "type": "added", "description": "Opening of an exposure file." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "added", "description": "Basic <a href=\"https://sed-ml.github.io/\">SED-ML</a> support (both through a <a href=\"https://sed-ml.github.io/\">SED-ML</a> file and a <a href=\"https://co.mbine.org/documents/archive\">COMBINE archive</a>)." },
                               { "type": "added", "description": "Creation of a new <a href=\"https://www.cellml.org/\">CellML</a> file or update of an existing one based on the results of a simulation." },
                               { "type": "added", "description": "Specification of the minimum/maximum values of the axes of a graph panel." },
                               { "type": "added", "description": "Synchronisation of the X and/or Y axes of all the graph panels." },
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "improved", "description": "Plotting of simulation data." },
                               { "type": "improved", "description": "Support for <a href=\"https://www.cellml.org/\">CellML</a> models." },
                               { "type": "improved", "description": "Listing of <a href=\"https://www.cellml.org/\">CellML</a> 1.1 model parameters." },
                               { "type": "fixed", "description": "Computing the initial value of a 'constant' algebraic variable." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "subCategory", "name": "<a href=\"https://computation.llnl.gov/projects/sundials/cvode\">CVODE</a> solver",
                             "entries": [
                               { "type": "improved", "description": "Parametrisation of the solver." }
                             ]
                           },
                           { "type": "subCategory", "name": "<a href=\"https://computation.llnl.gov/projects/sundials/ida\">IDA</a> solver",
                             "entries": [
                               { "type": "improved", "description": "Parametrisation of the solver." }
                             ]
                           }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "added", "description": "Support for <a href=\"https://sed-ml.github.io/\">SED-ML</a>." },
                           { "type": "added", "description": "Support for <a href=\"https://co.mbine.org/documents/archive\">COMBINE archive</a>." },
                           { "type": "subCategory", "name": "<a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> support",
                             "entries": [
                               { "type": "improved", "description": "Support for Unicode." }
                             ]
                           }
                         ]
                       },
                       { "name": "Tools",
                         "entries": [
                           { "type": "subCategory", "name": "CellML tools",
                             "entries": [
                               { "type": "improved", "description": "Export of a <a href=\"https://www.cellml.org/\">CellML</a> file." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.4.1", "description": "Version 0.4.1", "day": 20, "month": 5, "year": 2015,
                     "categories": [
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "Raw CellML view",
                             "entries": [
                               { "type": "improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "fixed", "description": "<a href=\"https://en.wikipedia.org/wiki/Mac_OS_X_Lion\">Mac OS X 10.7</a> (Lion) and <a href=\"https://en.wikipedia.org/wiki/OS_X_Mountain_Lion\">OS X 10.8</a> (Mountain Lion) issue with models that need to compute 10<sup>x</sup>." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.4", "description": "Version 0.4", "day": 29, "month": 4, "year": 2015,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "added", "description": "Checking for updates." },
                           { "type": "added", "description": "<a href=\"howToGetStarted.html\">How to get started</a> section." },
                           { "type": "improved", "description": "OpenCOR logo." },
                           { "type": "improved", "description": "Binaries on <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                           { "type": "improved", "description": "<a href=\"https://nsis.sourceforge.io/\">NSIS</a> packaging on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "improved", "description": "<a href=\"https://en.wikipedia.org/wiki/Zip_(file_format)\">ZIP</a> packaging on <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                           { "type": "improved", "description": "Handling of files." },
                           { "type": "improved", "description": "Display of numbers." },
                           { "type": "improved", "description": "Rendering of equations." },
                           { "type": "improved", "description": "Opening and reopening of files." }
                         ]
                       },
                       { "name": "Data store",
                         "entries": [
                           { "type": "added", "description": "<a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> data store." }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "added", "description": "Two new <a href=\"https://co.mbine.org/standards/qualifiers\">BioModels.net qualifiers</a> (<code>model:isInstanceOf</code> and <code>model:hasInstance</code>)." },
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "improved", "description": "Responsiveness of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "improved", "description": "Listing of component variables." },
                               { "type": "improved", "description": "Translation of views for inactive <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "improved", "description": "Setting of the modified state of the <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "improved", "description": "Detection of direct ontological terms." },
                               { "type": "improved", "description": "Retrieval of ontological terms." },
                               { "type": "improved", "description": "Listing of ontological terms." },
                               { "type": "fixed", "description": "Editing of malformed <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "fixed", "description": "Further editing after saving a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "fixed", "description": "Risk of the same <code>cmeta:id</code> value being used for two different <a href=\"https://www.cellml.org/\">CellML</a> elements." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw view",
                             "entries": [
                               { "type": "improved", "description": "Changes to file permissions." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw CellML view",
                             "entries": [
                               { "type": "added", "description": "Reformatting of the contents of the editor." },
                               { "type": "improved", "description": "Context menu." },
                               { "type": "improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "improved", "description": "Changes to file permissions." },
                               { "type": "fixed", "description": "Retrieval of view settings." }
                             ]
                           },
                           { "type": "added", "description": "<a href=\"user/plugins/editing/cellmlTextView.html\">CellML Text view</a>." }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "subCategory", "name": "Help",
                             "entries": [
                               { "type": "improved", "description": "Printing." }
                             ]
                           }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Model Repository window",
                             "entries": [
                               { "type": "improved", "description": "Retrieval of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "improved", "description": "Listing of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "improved", "description": "Export to <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>." },
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "fixed", "description": "Tracking of simulation properties." }
                             ]
                           }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "subCategory", "name": "CellML support",
                             "entries": [
                               { "type": "improved", "description": "Generation of error messages." },
                               { "type": "improved", "description": "Serialisation of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "improved", "description": "Reloading of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "fixed", "description": "Loading of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.3", "description": "Version 0.3", "day": 19, "month": 8, "year": 2014,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "added", "description": "Creation of a new blank file." },
                           { "type": "added", "description": "Creation of a new <a href=\"https://www.cellml.org/\">CellML</a> 1.0 file." },
                           { "type": "added", "description": "Creation of a new <a href=\"https://www.cellml.org/\">CellML</a> 1.1 file." },
                           { "type": "added", "description": "Opening of a remote file." },
                           { "type": "added", "description": "Duplication of a file." },
                           { "type": "added", "description": "(Un)locking of a file." },
                           { "type": "added", "description": "Closing of a file by pressing <code>Ctrl+W</code> on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a> on <a href=\"https://en.wikipedia.org/wiki/Linux\">Linux</a> and <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                           { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                           { "type": "improved", "description": "Plugin framework." },
                           { "type": "improved", "description": "Plugins dialog." },
                           { "type": "improved", "description": "Handling of files." },
                           { "type": "improved", "description": "Mode/view philosophy." },
                           { "type": "improved", "description": "Keyboard navigation in tree views." },
                           { "type": "improved", "description": "Saving of a file using any file extension." }
                         ]
                       },
                       { "name": "Development",
                         "entries": [
                           { "type": "subCategory", "name": "Examples of <a href=\"developer/develop/plugins/index.html\">how to write plugins</a>",
                             "entries": [
                               { "type": "added", "description": "<a href=\"developer/develop/plugins/sample.html\">Sample</a> plugin." },
                               { "type": "added", "description": "<a href=\"developer/develop/plugins/sampleTools.html\">Sample tools</a> plugin." },
                               { "type": "added", "description": "<a href=\"developer/develop/plugins/sampleView.html\">Sample view</a> plugin." },
                               { "type": "added", "description": "<a href=\"developer/develop/plugins/sampleWindow.html\">Sample window</a> plugin." }
                             ]
                           }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "improved", "description": "Retrieval of possible ontological terms." },
                               { "type": "fixed", "description": "Listing of some unrecognisable metadata." },
                               { "type": "fixed", "description": "Listing of suggested ontological terms." }
                             ]
                           },
                           { "type": "added", "description": "<a href=\"user/plugins/editing/rawTextView.html\">Raw view</a>." },
                           { "type": "added", "description": "<a href=\"user/plugins/editing/rawCellmlView.html\">Raw CellML view</a>." }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "subCategory", "name": "CellML tools",
                             "entries": [
                               { "type": "added", "description": "Export of a <a href=\"https://www.cellml.org/\">CellML</a> file to a user-defined format." },
                               { "type": "added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/C_(programming_language)\">C</a>." },
                               { "type": "added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/Fortran#FORTRAN_77\">FORTRAN 77</a>." },
                               { "type": "added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a>." },
                               { "type": "added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/Python_(programming_language)\">Python</a>." }
                             ]
                           }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Model Repository window",
                             "entries": [
                               { "type": "improved", "description": "Retrieval of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           },
                           { "type": "subCategory", "name": "File Browser window",
                             "entries": [
                               { "type": "added", "description": "Opening of a file by pressing <code>Enter</code> on (Mac) <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." }
                             ]
                           },
                           { "type": "subCategory", "name": "File Organiser window",
                             "entries": [
                               { "type": "added", "description": "Opening of a file by pressing <code>Enter</code> on (Mac) <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                               { "type": "fixed", "description": "Retrieval of the contents of the file organiser." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "improved", "description": "Delay between two data points." },
                               { "type": "improved", "description": "Information regarding a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "improved", "description": "Export to <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>." },
                               { "type": "fixed", "description": "Removal of a graph panel that is selected by another <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "fixed", "description": "Property settings of a <a href=\"https://www.cellml.org/\">CellML</a> file that requires a non-linear algebraic solver." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "subCategory", "name": "<a href=\"https://computation.llnl.gov/projects/sundials/cvode\">CVODE</a> solver",
                             "entries": [
                               { "type": "added", "description": "Choice to interpolate the solution." }
                             ]
                           },
                           { "type": "subCategory", "name": "<a href=\"https://computation.llnl.gov/projects/sundials/ida\">IDA</a> solver",
                             "entries": [
                               { "type": "added", "description": "Choice to interpolate the solution." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.2", "description": "Version 0.2", "day": 19, "month": 11, "year": 2013,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "added", "description": "<a href=\"licensing.html\">Open source license</a>." },
                           { "type": "added", "description": "Showing/hiding of all the recent/current docked widgets at once." },
                           { "type": "added", "description": "<a href=\"https://www.ncbi.nlm.nih.gov/pmc/articles/PMC1359535/\">Noble 1962</a> as one of the <a href=\"https://www.cellml.org/\">CellML</a> file examples." },
                           { "type": "added", "description": "Reloading of a file." },
                           { "type": "improved", "description": "Keyboard navigation in tree views." },
                           { "type": "improved", "description": "Handling of files." },
                           { "type": "improved", "description": "Plugins dialog." }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "added", "description": "Opening of an imported <a href=\"https://www.cellml.org/\">CellML</a> file." }
                             ]
                           }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "added", "description": "<a href=\"user/plugins/tools/cellmlTools.html\">CellML tools</a>." }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Model Repository window",
                             "entries": [
                               { "type": "improved", "description": "Retrieval of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "added", "description": "Ability to have several graph panels." },
                               { "type": "added", "description": "Plotting of any model parameter against any other." },
                               { "type": "added", "description": "Plotting of simulation data from another simulation." },
                               { "type": "improved", "description": "Various listings." },
                               { "type": "improved", "description": "Simulation speed." },
                               { "type": "improved", "description": "Graph panel features." },
                               { "type": "improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Retina_Display\">Retina displays</a>." },
                               { "type": "improved", "description": "Support for a wider range of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "added", "description": "<a href=\"https://en.wikipedia.org/wiki/Heun%27s_method\">Heun</a> solver." },
                           { "type": "added", "description": "Second-order <a href=\"https://en.wikipedia.org/wiki/Runge–Kutta_methods\">Runge-Kutta</a> solver." },
                           { "type": "added", "description": "Fourth-order <a href=\"https://en.wikipedia.org/wiki/Runge–Kutta_methods\">Runge-Kutta</a> solver." }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.1.2", "description": "Version 0.1.2", "day": 29, "month": 5, "year": 2013,
                     "categories": [
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "fixed", "description": "Generation of the runtime of some <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.1.1", "description": "Version 0.1.1", "day": 17, "month": 4, "year": 2013,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "improved", "description": "Plugin framework." }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "improved", "description": "Editing of property values." },
                               { "type": "fixed", "description": "Plotting of property values." }
                             ]
                           }
                         ]
                       }
                     ]
                   },
                   { "anchor": "0.1", "description": "Version 0.1", "day": 1, "month": 4, "year": 2013,
                     "categories": [
                       { "name": "Original version of OpenCOR" }
                     ]
                   }
                 ]
               };
