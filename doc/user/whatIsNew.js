var jsonData = { "versions": [
                   { "anchor": "latest", "description": "Latest snapshot", "day": 4, "month": 3, "year": 2017,
                     "categories": [
                       { "name": "General",
                         "entries": [
                           { "type": "Added", "description": "Preferences dialog." },
                           { "type": "Improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Linux\">Linux</a>." },
                           { "type": "Improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/MacOS\">macOS</a>." },
                           { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." }
                         ]
                       },
                       { "name": "Data store",
                         "entries": [
                           { "type": "Improved", "description": "Selection of data." }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "Improved", "description": "Conversion of <a href=\"https://www.cellml.org/\">CellML</a> files to the <a href=\"http://www.opencor.ws/user/plugins/editing/CellMLTextView.html#CellML Text format\">CellML Text format</a>." },
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "Fixed", "description": "Saving of a new file." }
                             ]
                           }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "Added", "description": "Web Browser window." }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "Added", "description": "<a href=\"plugins/organisation/PMRWorkspacesWindow.html\">PMR Workspaces window</a>." },
                           { "type": "subCategory", "name": "PMR window",
                             "entries": [
                               { "type": "Improved", "description": "Cloning of a workspace." }
                             ]
                           }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "subCategory", "name": "CellML support",
                             "entries": [
                               { "type": "Improved", "description": "Generation of error messages." }
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
                           { "type": "Added", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Windows_10\">Windows 10</a>." },
                           { "type": "Added", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/List_of_Ubuntu_releases#Ubuntu_16.04_LTS_.28Xenial_Xerus.29\">Ubuntu 16.04 LTS</a> (Xenial Xerus)." },
                           { "type": "Added", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/OS_X_El_Capitan\">OS X 10.11</a> (El Capitan) and <a href=\"https://en.wikipedia.org/wiki/MacOS_Sierra\">macOS 10.12</a> (Sierra)." },
                           { "type": "Added", "description": "Resetting all your settings from the <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a>." },
                           { "type": "Added", "description": "Detection of changes to one or several file dependencies." },
                           { "type": "Added", "description": "Support for an OpenCOR-specific URL scheme." },
                           { "type": "Added", "description": "Default view to open a particular type of file." },
                           { "type": "Added", "description": "Reference to the <a href=\"http://tutorial-on-cellml-opencor-and-pmr.readthedocs.org/en/latest/index.html\">official OpenCOR tutorial</a>." },
                           { "type": "Added", "description": "Use of a user's proxy settings." },
                           { "type": "Improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "Improved", "description": "Handling of files." },
                           { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                           { "type": "Improved", "description": "Rendering of equations." },
                           { "type": "Improved", "description": "Editing of text-based files." },
                           { "type": "Improved", "description": "Retrieval of a Save File name." },
                           { "type": "Improved", "description": "Reopening of the most recent file." },
                           { "type": "Fixed", "description": "Rendering of equations." },
                           { "type": "Fixed", "description": "Copying of a rendered equation to the clipboard (on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>)." },
                           { "type": "Fixed", "description": "Compiler engine crash due to an unfound external function (on <a href=\"https://en.wikipedia.org/wiki/Linux\">Linux</a>)." }
                         ]
                       },
                       { "name": "API",
                         "entries": [
                           { "type": "Added", "description": "<a href=\"http://sbml.org/\">SBML</a> API (via <a href=\"http://sbml.org/Software/libSBML/\">libSBML</a>)." },
                           { "type": "Added", "description": "<a href=\"http://www.sed-ml.org/\">SED-ML</a> API (via <a href=\"https://github.com/fbergmann/libSEDML/\">libSEDML</a>)." }
                         ]
                       },
                       { "name": "Data store",
                         "entries": [
                           { "type": "Added", "description": "BioSignalML data store." },
                           { "type": "subCategory", "name": "<a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> data store",
                             "entries": [
                               { "type": "Added", "description": "Selection of model parameters prior to data export." },
                               { "type": "Improved", "description": "Export of data." }
                             ]
                           }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw Text view (formerly known as the Raw view)",
                             "entries": [
                               { "type": "Improved", "description": "Support for text files." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw CellML view",
                             "entries": [
                               { "type": "Improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Fixed", "description": "Reformatting unescaped HTML entities." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "Added", "description": "Import/export of the <a href=\"http://www.opencor.ws/user/plugins/editing/CellMLTextView.html#CellML Text format\">CellML Text format</a> from/to <a href=\"https://www.cellml.org/\">CellML</a>." },
                               { "type": "Added", "description": "Support for the <code>min()</code>/<code>max()</code> and <code>gcd()</code>/<code>lcm()</code> functions." },
                               { "type": "Added", "description": "Offer to save the contents of the view to a text file if it cannot be saved as a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "Improved", "description": "Syntax highlighting." },
                               { "type": "Improved", "description": "Support for comments." },
                               { "type": "Improved", "description": "Support for <code>cmeta:id</code>'s." },
                               { "type": "Improved", "description": "Saving of a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "Fixed", "description": "Serialisation of a mathematical equation with a <code>cmeta:id</code> value." },
                               { "type": "Fixed", "description": "Serialisation of numbers that have an exponent." },
                               { "type": "Fixed", "description": "Serialisation of metadata." }
                             ]
                           },
                           { "type": "Added", "description": "<a href=\"plugins/editing/RawSEDMLView.html\">Raw SED-ML view</a>." }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "Physiome Model Repository window (formerly known as the CellML Model Repository window)",
                             "entries": [
                               { "type": "Improved", "description": "Listing of exposures and exposure files." },
                               { "type": "Added", "description": "Listing of <a href=\"http://www.sed-ml.org/\">SED-ML</a> files." },
                               { "type": "Added", "description": "Cloning of a workspace." },
                               { "type": "Added", "description": "Opening of an exposure file." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "Added", "description": "Basic <a href=\"http://www.sed-ml.org/\">SED-ML</a> support (both through a SED-ML file and a <a href=\"http://co.mbine.org/documents/archive\">COMBINE archive</a>)." },
                               { "type": "Added", "description": "Creation of a new <a href=\"https://www.cellml.org/\">CellML</a> file or update of an existing one based on the results of a simulation." },
                               { "type": "Added", "description": "Specification of the minimum/maximum values of the axes of a graph panel." },
                               { "type": "Added", "description": "Synchronisation of the X and/or Y axes of all the graph panels." },
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "Improved", "description": "Plotting of simulation data." },
                               { "type": "Improved", "description": "Support for <a href=\"https://www.cellml.org/\">CellML</a> models." },
                               { "type": "Improved", "description": "Listing of <a href=\"https://www.cellml.org/\">CellML</a> 1.1 model parameters." },
                               { "type": "Fixed", "description": "Computing the initial value of a 'constant' algebraic variable." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "subCategory", "name": "<a href=\"http://computation.llnl.gov/projects/sundials/cvode\">CVODE</a> solver",
                             "entries": [
                               { "type": "Improved", "description": "Parametrisation of the solver." }
                             ]
                           },
                           { "type": "subCategory", "name": "<a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> solver",
                             "entries": [
                               { "type": "Improved", "description": "Parametrisation of the solver." }
                             ]
                           }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "Added", "description": "Support for <a href=\"http://www.sed-ml.org/\">SED-ML</a>." },
                           { "type": "Added", "description": "Support for <a href=\"http://co.mbine.org/documents/archive\">COMBINE archive</a>." },
                           { "type": "subCategory", "name": "<a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> support",
                             "entries": [
                               { "type": "Improved", "description": "Support for Unicode." }
                             ]
                           }
                         ]
                       },
                       { "name": "Tools",
                         "entries": [
                           { "type": "subCategory", "name": "CellML tools",
                             "entries": [
                               { "type": "Improved", "description": "Export of a <a href=\"https://www.cellml.org/\">CellML</a> file." }
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
                               { "type": "Improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           },
                           { "type": "subCategory", "name": "CellML Text view",
                             "entries": [
                               { "type": "Improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "Fixed", "description": "<a href=\"https://en.wikipedia.org/wiki/Mac_OS_X_Lion\">Mac OS X 10.7</a> (Lion) and <a href=\"https://en.wikipedia.org/wiki/OS_X_Mountain_Lion\">OS X 10.8</a> (Mountain Lion) issue with models that need to compute 10<sup>x</sup>." }
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
                           { "type": "Added", "description": "Checking for updates." },
                           { "type": "Added", "description": "<a href=\"howToGetStarted.html\">How to get started</a> section." },
                           { "type": "Added", "description": "Support for 64-bit <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "Added", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/OS_X_Yosemite\">OS X 10.10</a> (Yosemite)." },
                           { "type": "Improved", "description": "OpenCOR logo." },
                           { "type": "Improved", "description": "Binaries on <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                           { "type": "Improved", "description": "<a href=\"http://nsis.sourceforge.net/\">NSIS</a> packaging on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "Improved", "description": "<a href=\"https://en.wikipedia.org/wiki/Zip_(file_format)\">ZIP</a> packaging on <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                           { "type": "Improved", "description": "Handling of files." },
                           { "type": "Improved", "description": "Display of numbers." },
                           { "type": "Improved", "description": "Rendering of equations." },
                           { "type": "Improved", "description": "Opening and reopening of files." }
                         ]
                       },
                       { "name": "Data store",
                         "entries": [
                           { "type": "Added", "description": "<a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> data store." }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "Added", "description": "Two new <a href=\"http://biomodels.net/qualifiers/\">BioModels.net qualifiers</a> (<code>model:isInstanceOf</code> and <code>model:hasInstance</code>)." },
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "Improved", "description": "Responsiveness of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "Improved", "description": "Listing of component variables." },
                               { "type": "Improved", "description": "Translation of views for inactive <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Improved", "description": "Setting of the modified state of the <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "Improved", "description": "Detection of direct ontological terms." },
                               { "type": "Improved", "description": "Retrieval of ontological terms." },
                               { "type": "Improved", "description": "Listing of ontological terms." },
                               { "type": "Fixed", "description": "Editing of malformed <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Fixed", "description": "Further editing after saving a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "Fixed", "description": "Risk of the same <code>cmeta:id</code> value being used for two different <a href=\"https://www.cellml.org/\">CellML</a> elements." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw view",
                             "entries": [
                               { "type": "Improved", "description": "Changes to file permissions." }
                             ]
                           },
                           { "type": "subCategory", "name": "Raw CellML view",
                             "entries": [
                               { "type": "Added", "description": "Reformatting of the contents of the editor." },
                               { "type": "Improved", "description": "Context menu." },
                               { "type": "Improved", "description": "Validation of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Improved", "description": "Changes to file permissions." },
                               { "type": "Fixed", "description": "Retrieval of view settings." }
                             ]
                           },
                           { "type": "Added", "description": "<a href=\"plugins/editing/CellMLTextView.html\">CellML Text view</a>." }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "subCategory", "name": "Help",
                             "entries": [
                               { "type": "Improved", "description": "Printing." }
                             ]
                           }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Model Repository window",
                             "entries": [
                               { "type": "Improved", "description": "Retrieval of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Improved", "description": "Listing of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "Improved", "description": "Export to <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>." },
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "Fixed", "description": "Tracking of simulation properties." }
                             ]
                           }
                         ]
                       },
                       { "name": "Support",
                         "entries": [
                           { "type": "subCategory", "name": "CellML support",
                             "entries": [
                               { "type": "Improved", "description": "Generation of error messages." },
                               { "type": "Improved", "description": "Serialisation of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Improved", "description": "Reloading of <a href=\"https://www.cellml.org/\">CellML</a> files." },
                               { "type": "Fixed", "description": "Loading of <a href=\"https://www.cellml.org/\">CellML</a> files." }
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
                           { "type": "Added", "description": "Creation of a new blank file." },
                           { "type": "Added", "description": "Creation of a new <a href=\"https://www.cellml.org/\">CellML</a> 1.0 file." },
                           { "type": "Added", "description": "Creation of a new <a href=\"https://www.cellml.org/\">CellML</a> 1.1 file." },
                           { "type": "Added", "description": "Opening of a remote file." },
                           { "type": "Added", "description": "Duplication of a file." },
                           { "type": "Added", "description": "(Un)locking of a file." },
                           { "type": "Added", "description": "Closing of a file by pressing <code>Ctrl+W</code> on <a href=\"https://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>." },
                           { "type": "Improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Command-line_interface\">CLI</a> on <a href=\"https://en.wikipedia.org/wiki/Linux\">Linux</a> and <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                           { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                           { "type": "Improved", "description": "Plugin framework." },
                           { "type": "Improved", "description": "Plugins window." },
                           { "type": "Improved", "description": "Handling of files." },
                           { "type": "Improved", "description": "Mode/view philosophy." },
                           { "type": "Improved", "description": "Keyboard navigation in tree views." },
                           { "type": "Improved", "description": "Saving of a file using any file extension." }
                         ]
                       },
                       { "name": "Development",
                         "entries": [
                           { "type": "subCategory", "name": "Examples of <a href=\"http://www.opencor.ws/developer/develop/plugins/\">how to write plugins</a>",
                             "entries": [
                               { "type": "Added", "description": "<a href=\"http://www.opencor.ws/developer/develop/plugins/Sample.html\">Sample</a> plugin." },
                               { "type": "Added", "description": "<a href=\"http://www.opencor.ws/developer/develop/plugins/SampleTools.html\">SampleTools</a> plugin." },
                               { "type": "Added", "description": "<a href=\"http://www.opencor.ws/developer/develop/plugins/SampleView.html\">SampleView</a> plugin." },
                               { "type": "Added", "description": "<a href=\"http://www.opencor.ws/developer/develop/plugins/SampleWindow.html\">SampleWindow</a> plugin." }
                             ]
                           }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "Improved", "description": "Retrieval of possible ontological terms." },
                               { "type": "Fixed", "description": "Listing of some unrecognisable metadata." },
                               { "type": "Fixed", "description": "Listing of suggested ontological terms." }
                             ]
                           },
                           { "type": "Added", "description": "<a href=\"plugins/editing/RawTextView.html\">Raw view</a>." },
                           { "type": "Added", "description": "<a href=\"plugins/editing/RawCellMLView.html\">Raw CellML view</a>." }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "subCategory", "name": "CellML tools",
                             "entries": [
                               { "type": "Added", "description": "Export of a <a href=\"https://www.cellml.org/\">CellML</a> file to a user-defined format." },
                               { "type": "Added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/C_(programming_language)\">C</a>." },
                               { "type": "Added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/Fortran#FORTRAN_77\">FORTRAN 77</a>." },
                               { "type": "Added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/MATLAB\">MATLAB</a>." },
                               { "type": "Added", "description": "User-defined format for <a href=\"https://en.wikipedia.org/wiki/Python_(programming_language)\">Python</a>." }
                             ]
                           }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Model Repository window",
                             "entries": [
                               { "type": "Improved", "description": "Retrieval of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           },
                           { "type": "subCategory", "name": "File Browser window",
                             "entries": [
                               { "type": "Added", "description": "Opening of a file by pressing <code>Enter</code> on (Mac) <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." }
                             ]
                           },
                           { "type": "subCategory", "name": "File Organiser window",
                             "entries": [
                               { "type": "Added", "description": "Opening of a file by pressing <code>Enter</code> on (Mac) <a href=\"https://en.wikipedia.org/wiki/OS_X\">OS X</a>." },
                               { "type": "Fixed", "description": "Retrieval of the contents of the file organiser." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "Improved", "description": "Rendering of the <a href=\"https://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a>." },
                               { "type": "Improved", "description": "Delay between two data points." },
                               { "type": "Improved", "description": "Information regarding a <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "Improved", "description": "Export to <a href=\"https://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>." },
                               { "type": "Fixed", "description": "Removal of a graph panel that is selected by another <a href=\"https://www.cellml.org/\">CellML</a> file." },
                               { "type": "Fixed", "description": "Property settings of a <a href=\"https://www.cellml.org/\">CellML</a> file that requires a non-linear algebraic solver." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "subCategory", "name": "<a href=\"http://computation.llnl.gov/projects/sundials/cvode\">CVODE</a> solver",
                             "entries": [
                               { "type": "Added", "description": "Choice to interpolate the solution." }
                             ]
                           },
                           { "type": "subCategory", "name": "<a href=\"http://computation.llnl.gov/projects/sundials/ida\">IDA</a> solver",
                             "entries": [
                               { "type": "Added", "description": "Choice to interpolate the solution." }
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
                           { "type": "Added", "description": "<a href=\"licensing.html\">Open source license</a>." },
                           { "type": "Added", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Mac_OS_X_Lion\">Mac OS X 10.7</a> (Lion) and later." },
                           { "type": "Added", "description": "Showing/hiding of all the recent/current docked widgets at once." },
                           { "type": "Added", "description": "<a href=\"https://www.ncbi.nlm.nih.gov/pmc/articles/PMC1359535/\">Noble 1962</a> as one of the <a href=\"https://www.cellml.org/\">CellML</a> file examples." },
                           { "type": "Added", "description": "Reloading of a file." },
                           { "type": "Improved", "description": "Keyboard navigation in tree views." },
                           { "type": "Improved", "description": "Handling of files." },
                           { "type": "Improved", "description": "Plugins window." }
                         ]
                       },
                       { "name": "Editing",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Annotation view",
                             "entries": [
                               { "type": "Added", "description": "Opening of an imported <a href=\"https://www.cellml.org/\">CellML</a> file." }
                             ]
                           }
                         ]
                       },
                       { "name": "Miscellaneous",
                         "entries": [
                           { "type": "Added", "description": "<a href=\"plugins/tools/CellMLTools.html\">CellML tools</a>." }
                         ]
                       },
                       { "name": "Organisation",
                         "entries": [
                           { "type": "subCategory", "name": "CellML Model Repository window",
                             "entries": [
                               { "type": "Improved", "description": "Retrieval of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "Added", "description": "Ability to have several graph panels." },
                               { "type": "Added", "description": "Plotting of any model parameter against any other." },
                               { "type": "Added", "description": "Plotting of simulation data from another simulation." },
                               { "type": "Improved", "description": "Various listings." },
                               { "type": "Improved", "description": "Simulation speed." },
                               { "type": "Improved", "description": "Graph panel features." },
                               { "type": "Improved", "description": "Support for <a href=\"https://en.wikipedia.org/wiki/Retina_Display\">Retina displays</a>." },
                               { "type": "Improved", "description": "Support for a wider range of <a href=\"https://www.cellml.org/\">CellML</a> files." }
                             ]
                           }
                         ]
                       },
                       { "name": "Solver",
                         "entries": [
                           { "type": "Added", "description": "<a href=\"https://en.wikipedia.org/wiki/Heun%27s_method\">Heun</a> solver." },
                           { "type": "Added", "description": "Second-order <a href=\"https://en.wikipedia.org/wiki/Runge–Kutta_methods\">Runge-Kutta</a> solver." },
                           { "type": "Added", "description": "Fourth-order <a href=\"https://en.wikipedia.org/wiki/Runge–Kutta_methods\">Runge-Kutta</a> solver." }
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
                               { "type": "Fixed", "description": "Generation of the runtime of some <a href=\"https://www.cellml.org/\">CellML</a> files." }
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
                           { "type": "Improved", "description": "Plugin framework." }
                         ]
                       },
                       { "name": "Simulation",
                         "entries": [
                           { "type": "subCategory", "name": "Single Cell view",
                             "entries": [
                               { "type": "Improved", "description": "Editing of property values." },
                               { "type": "Fixed", "description": "Plotting of property values." }
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
