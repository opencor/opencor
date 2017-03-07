var jsonData = { "versions": [
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
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://khan.github.io/KaTeX/\">KaTeX</a> to version 0.7.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1195\">#1195</a>). Upgraded <a href=\"http://www.llvm.org/\">LLVM</a> to version 3.9.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1099\">#1099</a>). Upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.25.1 (see issue <a href=\"https://github.com/opencor/opencor/issues/1190\">#1190</a>). Upgraded <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2k (see issue <a href=\"https://github.com/opencor/opencor/issues/1202\">#1202</a>). Upgraded <a href=\"https://riverbankcomputing.com/software/qscintilla/intro\">QScintilla</a> to version 2.9.4 (see issue <a href=\"https://github.com/opencor/opencor/issues/1179\">#1179</a>). Upgraded <a href=\"http://www.zlib.net/\">zlib</a> to version 1.2.11 (see issue <a href=\"https://github.com/opencor/opencor/issues/1193\">#1193</a>)." }
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
