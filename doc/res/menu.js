function headerAndContentsMenu(pageName, relativePath) {
    doHeaderAndContentsMenu(pageName, relativePath, 103, 103, 103,
                            { "menuItems": [
                                { "level": 0, "label": "Home", "link": "index.html" },
                                { "separator": true },
                                { "level": 0, "label": "User documentation", "link": "user" },
                                { "level": 0, "label": "Developer documentation", "link": "developer" },
                                { "separator": true },
                                { "level": 0, "label": "Downloads", "link": "downloads" },
                                { "level": 1, "label": "Previous snapshots", "link": "downloads/previousSnapshots.html" },
                                { "separator": true },
                                { "level": 0, "label": "Contact us", "link": "user/contactUs.html" }
                              ]
                            });
}
