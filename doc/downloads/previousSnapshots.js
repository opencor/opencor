var jsonData = { "versions": [
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
                       { "change": "<strong>Graph Panel widget:</strong> improved the plotting of small values (see issue <a href=\"https://github.com/opencor/opencor/issues/1322\">#1322</a>)." },
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
                   },
                 ]
               };
