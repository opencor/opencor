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
                       { "change": "<strong>General:</strong> fixed a problem with OpenCOR firing up an 'old' version of OS X, but not opening the CellML/SED-ML file referenced in an OpenCOR URL (see issue <a href=\"https://github.com/opencor/opencor/issues/909\">#909</a>)." },
                       { "change": "<strong>Third-party libraries:</strong> upgraded <a href=\"https://libgit2.github.com/\">libgit2</a> to version 0.24 (see issue <a href=\"https://github.com/opencor/opencor/issues/914\">#914</a>) and <a href=\"https://www.openssl.org/\">OpenSSL</a> to version 1.0.2g (Windows only; see issue <a href=\"https://github.com/opencor/opencor/issues/910\">#910</a>)." }
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
