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
