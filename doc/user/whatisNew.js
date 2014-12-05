var jsonData = {"versions":[
                   {"anchor":"latest", "description":"Latest snapshot", "date":"5 December 2014",
                    "categories":[
                        {"name":"General",
                         "entries":[
                             {"type":"Added", "description":"Support for 64-bit <a href=\"http://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>."},
                             {"type":"Added", "description":"Support for 64-bit <a href=\"http://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>."},
                             {"type":"Added", "description":"Checking for updates."},
                             {"type":"Improved", "description":"OpenCOR logo."},
                             {"type":"Improved", "description":"Binaries on <a href=\"http://en.wikipedia.org/wiki/OS_X\">OS X</a>."},
                             {"type":"Improved", "description":"<a href=\"http://en.wikipedia.org/wiki/Zip_(file_format)\">ZIP</a> packaging on <a href=\"http://en.wikipedia.org/wiki/OS_X\">OS X</a>."},
                             {"type":"Improved", "description":"Displaying of numbers."}
                         ]
                        },
                        {"name":"Data store",
                         "entries":[
                             {"type":"Added", "description":"<a href=\"http://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a> data store."}
                         ]
                        },
                        {"name":"Editing",
                         "entries":[
                             {"type":"subCategory", "name":"CellML annotation view",
                              "entries":[
                                  {"type":"Added", "description":"Two new <a href=\"http://biomodels.net/qualifiers/\">BioModels.net qualifiers</a> (<code>model:isInstanceOf</code> and <code>model:hasInstance</code>)."},
                                  {"type":"Improved", "description":"GUI for locked CellML files."},
                                  {"type":"Improved", "description":"Responsiveness of the GUI."},
                                  {"type":"Improved", "description":"Listing of component variables."},
                                  {"type":"Improved", "description":"Translation of views for inactive CellML files."},
                                  {"type":"Improved", "description":"Setting of the modified state of the CellML file."},
                                  {"type":"Improved", "description":"Detection of direct ontological terms."},
                                  {"type":"Improved", "description":"Retrieval of ontological terms."},
                                  {"type":"Improved", "description":"Listing of ontological terms."},
                                  {"type":"Fixed", "description":"Editing a malformed CellML files."},
                                  {"type":"Fixed", "description":"Further editing after saving a CellML file."}
                              ]
                             },
                             {"type":"subCategory", "name":"Raw CellML view",
                              "entries":[
                                  {"type":"Improved", "description":"Context menu."},
                                  {"type":"Fixed", "description":"Retrieval of view settings."}
                              ]
                             }
                         ]
                        },
                        {"name":"Organisation",
                         "entries":[
                             {"type":"subCategory", "name":"CellML Model Repository window",
                              "entries":[
                                  {"type":"Improved", "description":"Retrieval of CellML files."},
                                  {"type":"Improved", "description":"Listing of CellML files."}
                              ]
                             }
                         ]
                        },
                        {"name":"Simulation",
                         "entries":[
                             {"type":"subCategory", "name":"Single cell (simulation) view",
                              "entries":[
                                  {"type":"Improved", "description":"Export to <a href=\"http://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>."},
                                  {"type":"Improved", "description":"GUI for the parameters section."}
                              ]
                             }
                         ]
                        },
                        {"name":"Support",
                         "entries":[
                             {"type":"subCategory", "name":"CellML support",
                              "entries":[
                                  {"type":"Improved", "description":"Serialisation of CellML files."}
                              ]
                             }
                         ]
                        }
                    ]
                   },
                   {"anchor":"0.3", "description":"Version 0.3", "date":"19 August 2014",
                    "categories":[
                        {"name":"General",
                         "entries":[
                             {"type":"Added", "description":"Create a new blank file."},
                             {"type":"Added", "description":"Create a new CellML 1.0 file."},
                             {"type":"Added", "description":"Create a new CellML 1.1 file."},
                             {"type":"Added", "description":"Open a remote file."},
                             {"type":"Added", "description":"Duplicate a file."},
                             {"type":"Added", "description":"(Un)lock a file."},
                             {"type":"Added", "description":"Close a file by pressing <code>Ctrl+W</code> on <a href=\"http://en.wikipedia.org/wiki/Microsoft_Windows\">Windows</a>."},
                             {"type":"Improved", "description":"<a href=\"http://en.wikipedia.org/wiki/Command-line_interface\">CLI</a> support on Linux and <a href=\"http://en.wikipedia.org/wiki/OS_X\">OS X</a>."},
                             {"type":"Improved", "description":"<a href=\"userInterfaces/commandLineInterface.html\">CLI</a> with improved <a href=\"plugins/miscellaneous/CellMLTools.html\">CellML tools</a>."},
                             {"type":"Improved", "description":"<a href=\"http://en.wikipedia.org/wiki/Graphical_user_interface\">GUI</a> support."},
                             {"type":"Improved", "description":"Plugin framework."},
                             {"type":"Improved", "description":"Plugins window."},
                             {"type":"Improved", "description":"File handling."},
                             {"type":"Improved", "description":"Mode/view philosophy."},
                             {"type":"Improved", "description":"Keyboard navigation in tree views."},
                             {"type":"Improved", "description":"Save a file using any file extension."}
                         ]
                        },
                        {"name":"Development",
                         "entries":[
                             {"type":"subCategory", "name":"Examples of <a href=\"http://www.opencor.ws/developer/develop/plugins/index.html\">how to write plugins</a>",
                              "entries":[
                                  {"type":"Added", "description":"<a href=\"http://www.opencor.ws/developer/develop/plugins/Sample.html\">Sample</a> plugin."},
                                  {"type":"Added", "description":"<a href=\"http://www.opencor.ws/developer/develop/plugins/SampleTools.html\">SampleTools</a> plugin."},
                                  {"type":"Added", "description":"<a href=\"http://www.opencor.ws/developer/develop/plugins/SampleView.html\">SampleView</a> plugin."},
                                  {"type":"Added", "description":"<a href=\"http://www.opencor.ws/developer/develop/plugins/SampleWindow.html\">SampleWindow</a> plugin."}
                              ]
                             }
                         ]
                        },
                        {"name":"Editing",
                         "entries":[
                             {"type":"subCategory", "name":"CellML annotation view",
                              "entries":[
                                  {"type":"Improved", "description":"Retrieval of possible ontological terms."},
                                  {"type":"Fixed", "description":"Listing of some unrecognisable metadata."},
                                  {"type":"Fixed", "description":"Listing of suggested ontological terms."}
                              ]
                             },
                             {"type":"Added", "description":"<a href=\"plugins/editing/RawView.html\">Raw view</a>."},
                             {"type":"Added", "description":"<a href=\"plugins/editing/RawCellMLView.html\">Raw CellML view</a>."}
                         ]
                        },
                        {"name":"Miscellaneous",
                         "entries":[
                             {"type":"subCategory", "name":"CellML tools",
                              "entries":[
                                  {"type":"Added", "description":"Export of a CellML file to a user-defined format."},
                                  {"type":"Added", "description":"User-defined format for <a href=\"http://en.wikipedia.org/wiki/C_(programming_language)\">C</a>."},
                                  {"type":"Added", "description":"User-defined format for <a href=\"http://en.wikipedia.org/wiki/Fortran#FORTRAN_77\">FORTRAN 77</a>."},
                                  {"type":"Added", "description":"User-defined format for <a href=\"http://en.wikipedia.org/wiki/MATLAB\">MATLAB</a>."},
                                  {"type":"Added", "description":"User-defined format for <a href=\"http://en.wikipedia.org/wiki/Python_(programming_language)\">Python</a>."}
                              ]
                             }
                         ]
                        },
                        {"name":"Organisation",
                         "entries":[
                             {"type":"subCategory", "name":"CellML Model Repository window",
                              "entries":[
                                  {"type":"Improved", "description":"Retrieval of CellML files."}
                              ]
                             },
                             {"type":"subCategory", "name":"File Browser window",
                              "entries":[
                                  {"type":"Added", "description":"Open a file by pressing <code>Enter</code> on (Mac) <a href=\"http://en.wikipedia.org/wiki/OS_X\">OS X</a>."}
                              ]
                             },
                             {"type":"subCategory", "name":"File Organiser window",
                              "entries":[
                                  {"type":"Added", "description":"Open a file by pressing <code>Enter</code> on (Mac) <a href=\"http://en.wikipedia.org/wiki/OS_X\">OS X</a>."},
                                  {"type":"Fixed", "description":"Retrieval of the contents of the file organiser."}
                              ]
                             }
                         ]
                        },
                        {"name":"Simulation",
                         "entries":[
                             {"type":"subCategory", "name":"Single cell (simulation) view",
                              "entries":[
                                  {"type":"Improved", "description":"View settings upon reloading a CellML file."},
                                  {"type":"Improved", "description":"Delay between two data points."},
                                  {"type":"Improved", "description":"CellML file information."},
                                  {"type":"Improved", "description":"Export to <a href=\"http://en.wikipedia.org/wiki/Comma-separated_values\">CSV</a>."},
                                  {"type":"Fixed", "description":"Removal of a graph panel that is selected by another CellML file."},
                                  {"type":"Fixed", "description":"Property settings of a CellML file that requires a non-linear algebraic solver."}
                              ]
                             },
                             {"type":"subCategory", "name":"<a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_cvode\">CVODE</a> solver",
                              "entries":[
                                  {"type":"Added", "description":"Choice to interpolate the solution."}
                              ]
                             },
                             {"type":"subCategory", "name":"<a href=\"http://computation.llnl.gov/casc/sundials/description/description.html#descr_ida\">IDA</a> solver",
                              "entries":[
                                  {"type":"Added", "description":"Choice to interpolate the solution."}
                              ]
                             }
                         ]
                        }
                    ]
                   },
                   {"anchor":"0.2", "description":"Version 0.2", "date":"19 November 2013",
                    "categories":[
                        {"name":"General",
                         "entries":[
                             {"type":"Added", "description":"<a href=\"licensing.html\">Open source license</a>."},
                             {"type":"Added", "description":"Support for Mac <a href=\"http://en.wikipedia.org/wiki/OS_X\">OS X</a> 10.7 (Lion) and later."},
                             {"type":"Added", "description":"Reload a file (either as a result of a direct action or because it got modified outside OpenCOR)."},
                             {"type":"Added", "description":"Show/hide all the recent/current docked widgets at once."},
                             {"type":"Added", "description":"<a href=\"http://www.ncbi.nlm.nih.gov/pmc/articles/PMC1359535/\">Noble 1962</a> as one of the CellML file examples."},
                             {"type":"Improved", "description":"Keyboard navigation in tree views."},
                             {"type":"Improved", "description":"<a href=\"userInterfaces/commandLineInterface.html\">CLI</a> with <a href=\"plugins/miscellaneous/CellMLTools.html\">CellML tools</a>."},
                             {"type":"Improved", "description":"Plugins window."},
                             {"type":"Improved", "description":"File handling."}
                         ]
                        },
                        {"name":"Editing",
                         "entries":[
                             {"type":"subCategory", "name":"CellML annotation view",
                              "entries":[
                                  {"type":"Added", "description":"Open an imported CellML file."}
                              ]
                             }
                         ]
                        },
                        {"name":"Miscellaneous",
                         "entries":[
                             {"type":"Added", "description":"<a href=\"plugins/miscellaneous/CellMLTools.html\">CellML tools</a>."}
                         ]
                        },
                        {"name":"Organisation",
                         "entries":[
                             {"type":"subCategory", "name":"CellML Model Repository window",
                              "entries":[
                                  {"type":"Improved", "description":"Retrieval of CellML files."}
                              ]
                             }
                         ]
                        },
                        {"name":"Simulation",
                         "entries":[
                             {"type":"Added", "description":"<a href=\"http://en.wikipedia.org/wiki/Heun%27s_method\">Heun</a> solver."},
                             {"type":"Added", "description":"Second-order <a href=\"http://en.wikipedia.org/wiki/Runge–Kutta_methods\">Runge-Kutta</a> solver."},
                             {"type":"Added", "description":"Fourth-order <a href=\"http://en.wikipedia.org/wiki/Runge–Kutta_methods\">Runge-Kutta</a> solver."},
                             {"type":"subCategory", "name":"Single cell (simulation) view",
                              "entries":[
                                  {"type":"Added", "description":"Have several graph panels."},
                                  {"type":"Added", "description":"Plot any model parameter against any other."},
                                  {"type":"Added", "description":"Plot simulation data from another simulation."},
                                  {"type":"Improved", "description":"Various listings."},
                                  {"type":"Improved", "description":"Simulation speed."},
                                  {"type":"Improved", "description":"Graph panel features."},
                                  {"type":"Improved", "description":"Support for <a href=\"http://en.wikipedia.org/wiki/Retina_Display\">Retina displays</a>."},
                                  {"type":"Improved", "description":"Support for a wider range of CellML files."}
                              ]
                             }
                         ]
                        }
                    ]
                   },
                   {"anchor":"0.1.2", "description":"Version 0.1.2", "date":"29 May 2013",
                    "categories":[
                        {"name":"Simulation",
                         "entries":[
                             {"type":"subCategory", "name":"Single cell (simulation) view",
                              "entries":[
                                  {"type":"Fixed", "description":"Runtime generation of some CellML files."}
                              ]
                             }
                         ]
                        }
                    ]
                   },
                   {"anchor":"0.1.1", "description":"Version 0.1.1", "date":"17 April 2013",
                    "categories":[
                        {"name":"General",
                         "entries":[
                             {"type":"Improved", "description":"Plugin framework."}
                         ]
                        },
                        {"name":"Simulation",
                         "entries":[
                             {"type":"subCategory", "name":"Single cell (simulation) view",
                              "entries":[
                                  {"type":"Improved", "description":"Editing of property values."},
                                  {"type":"Fixed", "description":"Plotting of property values."}
                              ]
                             }
                         ]
                        }
                    ]
                   },
                   {"anchor":"0.1", "description":"Version 0.1", "date":"1 April 2013",
                    "categories":[
                        {"name":"Original version of OpenCOR"
                        }
                    ]
                   }
                ]
               };
