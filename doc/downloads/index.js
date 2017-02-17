var jsonData = { "versions": [
                   { "major": 0, "minor": 5, "patch": 0, "day": 15, "month": 10, "year": 2016, "type": 1,
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
                       { "change": "<strong>PMR support:</strong> let the user know if we simply cannot authenticate (see issue <a href=\"https://github.com/opencor/opencor/issues/1223\">#1223</a>)." },
                       { "change": "<strong>PMR-based windows:</strong> only reload them (upon changes to preferences) if the PMR URL is different (see issue <a href=\"https://github.com/opencor/opencor/issues/1222\">#1222</a>)." }
                     ]
                   },
                   { "major": 0, "minor": 4, "patch": 1, "day": 20, "month": 5, "year": 2015, "type": 0,
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
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
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
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
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
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
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
                       { "name": "macOS", "supported": "Mac OS X 10.7 (Lion) and later",
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
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
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
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
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
                       { "name": "macOS", "supported": "OS X 10.8 (Mountain Lion) and later",
                         "files": [
                           { "extension": ".dmg" }, { "extension": ".zip" }
                         ]
                       }
                     ]
                   }
                 ]
               };
