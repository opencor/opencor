function headerAndContentsMenu(pageName, relativePath) {
    doHeaderAndContentsMenu(pageName, relativePath, 54, 96, 146,
                            { "menuItems": [
                                { "level": 0, "label": "Home", "link": "user/index.html" },
                                { "separator": true },
                                { "level": 0, "label": "How to get started?", "link": "user/howToGetStarted.html" },
                                { "separator": true },
                                { "level": 0, "label": "What is new?", "link": "user/whatIsNew.html" },
                                { "separator": true },
                                { "level": 0, "label": "User interfaces", "link": "user/userInterfaces/index.html" },
                                { "level": 1, "label": "Command line interface (CLI)", "link": "user/userInterfaces/commandLineInterface.html" },
                                { "level": 1, "label": "Graphical user interface (GUI)", "link": "user/userInterfaces/graphicalUserInterface.html" },
                                { "separator": true },
                                { "level": 0, "label": "Plugins", "link": "user/plugins/index.html" },
                                { "level": 1, "label": "Data Store", "subMenu": [
                                    { "level": 2, "label": "CSVDataStore", "link": "user/plugins/dataStore/CSVDataStore.html" },
                                  ]
                                },
                                { "level": 1, "label": "Editing", "subMenu": [
                                    { "level": 2, "label": "CellMLAnnotationView", "link": "user/plugins/editing/CellMLAnnotationView.html" },
                                    { "level": 2, "label": "CellMLTextView", "link": "user/plugins/editing/CellMLTextView.html" },
                                    { "level": 2, "label": "RawCellMLView", "link": "user/plugins/editing/RawCellMLView.html" },
                                    { "level": 2, "label": "RawView", "link": "user/plugins/editing/RawView.html" },
                                  ]
                                },
                                { "level": 1, "label": "Miscellaneous", "subMenu": [
                                    { "level": 2, "label": "CellMLTools", "link": "user/plugins/miscellaneous/CellMLTools.html" },
                                    { "level": 2, "label": "HelpWindow", "link": "user/plugins/miscellaneous/HelpWindow.html" },
                                  ]
                                },
                                { "level": 1, "label": "Organisation", "subMenu": [
                                    { "level": 2, "label": "CellMLModelRepositoryWindow", "link": "user/plugins/organisation/CellMLModelRepositoryWindow.html" },
                                    { "level": 2, "label": "FileBrowserWindow", "link": "user/plugins/organisation/FileBrowserWindow.html" },
                                    { "level": 2, "label": "FileOrganiserWindow", "link": "user/plugins/organisation/FileOrganiserWindow.html" },
                                  ]
                                },
                                { "level": 1, "label": "Simulation", "subMenu": [
                                    { "level": 2, "label": "SingleCellView", "link": "user/plugins/simulation/SingleCellView.html" },
                                  ]
                                },
                                { "level": 1, "label": "Solver", "subMenu": [
                                    { "level": 2, "label": "CVODESolver", "link": "user/plugins/solver/CVODESolver.html" },
                                    { "level": 2, "label": "ForwardEulerSolver", "link": "user/plugins/solver/ForwardEulerSolver.html" },
                                    { "level": 2, "label": "FourthOrderRungeKuttaSolver", "link": "user/plugins/solver/FourthOrderRungeKuttaSolver.html" },
                                    { "level": 2, "label": "HeunSolver", "link": "user/plugins/solver/HeunSolver.html" },
                                    { "level": 2, "label": "IDASolver", "link": "user/plugins/solver/IDASolver.html" },
                                    { "level": 2, "label": "KINSOLSolver", "link": "user/plugins/solver/KINSOLSolver.html" },
                                    { "level": 2, "label": "SecondOrderRungeKuttaSolver", "link": "user/plugins/solver/SecondOrderRungeKuttaSolver.html" },
                                  ]
                                },
                                { "separator": true },
                                { "level": 0, "label": "Known issues", "link": "user/knownIssues.html" },
                                { "level": 0, "label": "Known limitations", "link": "user/knownLimitations.html" },
                                { "separator": true },
                                { "level": 0, "label": "Supported platforms", "link": "user/supportedPlatforms.html" },
                                { "separator": true },
                                { "level": 0, "label": "Licensing", "link": "user/licensing.html" },
                                { "separator": true },
                                { "level": 0, "label": "Team", "link": "user/team.html" },
                                { "separator": true },
                                { "level": 0, "label": "Published article", "link": "http://dx.doi.org/10.3389/fphys.2015.00026", "directLink": true },
                                { "separator": true },
                                { "level": 0, "label": "Contact us", "link": "user/contactUs.html" },
                              ]
                            });
}
