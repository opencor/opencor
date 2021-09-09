function showContentsSubMenu(subMenuButton, showSubMenu) {
    subMenuButton.removeClass(showSubMenu?"subMenuClosed":"subMenuOpened");
    subMenuButton.addClass(showSubMenu?"subMenuOpened":"subMenuClosed");

    $("ul.contentsMenu > li > ul > li.subMenuItem#"+subMenuButton.attr("id")).css("display", showSubMenu?"list-item":"none");
}

function doHeaderAndContentsMenu(pageName, relativePath, data) {
    // Header

    document.write("<div class=\"header\">\n");
    document.write("    "+pageName+"\n");
    document.write("</div>\n");
    document.write("\n");

    // CSS styling for our contents menu

    document.write("<style>\n");
    document.write("    ul.contentsMenu {\n");
    document.write("        z-index: 999;\n");
    document.write("        -khtml-user-select: none;\n");
    document.write("        -moz-user-select: none;\n");
    document.write("        -ms-user-select: none;\n");
    document.write("        -webkit-touch-callout: none;\n");
    document.write("        -webkit-user-select: none;\n");
    document.write("        user-select: none;\n");
    document.write("        margin: 0px;\n");
    document.write("        position: fixed;\n");
    document.write("        top: 3px;\n");
    document.write("        right: 3px;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li,\n");
    document.write("    ul.contentsMenu > li > ul > li {\n");
    document.write("        list-style: none;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul {\n");
    document.write("        visibility: hidden;\n");
    document.write("        padding: 0px;\n");
    document.write("        position: fixed;\n");
    document.write("        top: 29px;\n");
    document.write("        right: 0px;\n");
    document.write("        background: rgb(249, 249, 249);\n");
    document.write("        border: 2px solid rgb(103, 103, 103);\n");
    document.write("        border-top-style: none;\n");
    document.write("        border-radius: 0px 0px 5px 5px;\n");
    document.write("        box-shadow: 0px 5px 5px -5px rgb(103, 103, 103);\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li {\n");
    document.write("        cursor: default;\n");
    document.write("        margin: 1px;\n");
    document.write("        font-size: 9pt;\n");
    document.write("        font-weight: normal;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.clickableMenuItem:hover {\n");
    document.write("        background: rgba(103, 103, 103, 0.79);\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.clickableMenuItem:hover div > a {\n");
    document.write("        color: rgb(255, 255, 255);\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.lastMenuItem.clickableMenuItem > div:hover,\n");
    document.write("    ul.contentsMenu > li > ul > li div.lastMenuItem.selectedMenuItem > div a {\n");
    document.write("        border-radius: 0px 0px 3px 3px;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.menuItemLabel {\n");
    document.write("        display: table-cell;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.menuItemTable {\n");
    document.write("        display: table;\n");
    document.write("        width: 100%;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.menuItemTableRow {\n");
    document.write("        display: table-row;\n");
    document.write("        vertical-align: middle;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.nonClickableMenuItem > div a {\n");
    document.write("        cursor: default;\n");
    document.write("        background: rgb(249, 249, 249);\n");
    document.write("        color: rgb(0, 0, 0);\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.selectedMenuItem > div a {\n");
    document.write("        cursor: default;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.selectedMenuItem > div a,\n");
    document.write("    ul.contentsMenu > li > ul > li div.selectedMenuItem > div.subMenuButton {\n");
    document.write("        background: rgba(103, 103, 103, 0.13);\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu > li > ul > li div.subMenuButton {\n");
    document.write("        cursor: pointer;\n");
    document.write("        display: table-cell;\n");
    document.write("        width: 1px;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu li ul li a {\n");
    document.write("        display: block;\n");
    document.write("        padding: 1px;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu li ul li a:hover {\n");
    document.write("        margin: 0px;\n");
    document.write("        border: 0px;\n");
    document.write("    }\n");
    document.write("\n");
    document.write("    ul.contentsMenu li ul li.separator {\n");
    document.write("        margin: 0px;\n");
    document.write("        padding: 0px;\n");
    document.write("        border-top: 1px solid rgb(103, 103, 103);\n");
    document.write("    }\n");
    document.write("</style>\n");
    document.write("\n");

    // Contents menu

    document.write("<ul class=\"contentsMenu\">\n");
    document.write("    <li>\n");
    document.write("        <img src=\""+relativePath+"/res/pics/oxygen/actions/help-about.png\" width=24 height=24>\n");
    document.write("        <ul>\n");

    let menuItems = data.menuItems;
    let subMenuCounter = 0;
    let subMenuSelected = 0;

    for (let i = 0; i < menuItems.length; ++i) {
        let menuItem = menuItems[i];

        if (menuItem.separator) {
            // We are dealing with a menu separator

            document.write("            <li class=\"separator\"></li>\n");
        } else {
            // We are dealing with a menu item

            let selectedMenuItem = false;

            if (   (   (typeof menuItem.label !== "undefined")
                    && (menuItem.label.toLowerCase() === pageName.toLowerCase()))
                || ((pageName === "OpenCOR") && (menuItem.label === "Home"))
                || (pageName === menuItem.label+" Plugin")) {
                selectedMenuItem = true;
            }

            let liId = "";
            let liClass = "";

            if (   (typeof menuItem.subMenuItem !== "undefined")
                &&  menuItem.subMenuItem) {
                liId = "subMenu"+subMenuCounter;
                liClass = " class=\"subMenuItem\"";
            }

            let menuItemIndent = "";

            for (let j = 0; j < menuItem.level; ++j) {
                menuItemIndent += "&nbsp;&nbsp;&nbsp;&nbsp;"
            }

            let menuItemLink = "";

            if (   (typeof menuItem.link !== "undefined")
                &&  menuItem.link.length) {
                menuItemLink = menuItem.directLink?menuItem.link:relativePath+"/"+menuItem.link;
            }

            let tableRowClasses = "menuItemTableRow";

            if (i === menuItems.length-1) {
                tableRowClasses += " lastMenuItem";
            }

            if (selectedMenuItem) {
                tableRowClasses += " selectedMenuItem";

                if (liId.length) {
                    subMenuSelected = subMenuCounter;
                }
            } else {
                if (menuItemLink.length) {
                    tableRowClasses += " clickableMenuItem";
                } else {
                    tableRowClasses += " nonClickableMenuItem";
                }
            }

            let subMenuButton = "";

            if (   (typeof menuItem.subMenuHeader !== "undefined")
                &&  menuItem.subMenuHeader) {
                subMenuButton = "<div id=\"subMenu"+(++subMenuCounter)+"\" class=\"subMenuButton subMenuClosed\">...</div>";
            }

            document.write("            <li"+(liId.length?" id=\""+liId+"\"":"")+liClass+">\n");
            document.write("                <div class=\"menuItemTable\">\n");
            document.write("                    <div class=\""+tableRowClasses+"\">\n");
            document.write("                        <div class=\"menuItemLabel\">\n");
            document.write("                            <a href=\""+menuItemLink+"\">"+menuItemIndent+menuItem.label+"</a>\n");
            document.write("                        </div>\n");

            if (subMenuButton.length) {
                document.write("                        "+subMenuButton+"\n");
            }

            document.write("                    </div>\n");
            document.write("                </div>\n");
            document.write("            </li>\n");
        }
    }

    document.write("        </ul>\n");
    document.write("    </li>\n");
    document.write("</ul>\n");

    // Keep track of our number of sub-menus

    $("ul.contentsMenu > li > ul").attr("subMenus", subMenuCounter);

    // Show/hide our sub-menus, depending on whether one of them contains the
    // selected menu item

    for (let i = 1; i <= subMenuCounter; ++i) {
        let subMenuButton = $("ul.contentsMenu > li > ul > li > div > div > div#subMenu"+i);

        if (i === subMenuSelected) {
            subMenuButton.css("display", "none");
        }

        showContentsSubMenu(subMenuButton, i === subMenuSelected);
    }

    // Show/hide our contents menu

    $(document).click(function() {
        $("ul.contentsMenu > li > ul").css("visibility", "hidden");
    });

    $("ul.contentsMenu > li > img").click(function(event) {
        if ($("ul.contentsMenu > li > ul").css("visibility") === "visible") {
            $("ul.contentsMenu > li > ul").css("visibility", "hidden");
        } else {
            for (let i = 1; i <= $("ul.contentsMenu > li > ul").attr("subMenus"); ++i) {
                showContentsSubMenu($("ul.contentsMenu > li > ul > li > div > div > div#subMenu"+i), i === subMenuSelected);
            }

            $("ul.contentsMenu > li > ul").css("visibility", "visible");
        }

        event.stopPropagation();
    });

    // Prevent our contents menu from being hidden when clicking on it or on a
    // menu item that is selected or non-clickable

    $("ul.contentsMenu > li > ul").click(function() {
        return false;
    });

    $("ul.contentsMenu > li > ul > li > div > div.selectedMenuItem > div > a").click(function() {
        return false;
    });

    $("ul.contentsMenu > li > ul > li > div > div.nonClickableMenuItem > div > a").click(function() {
        return false;
    });

    // Hide our contents menu if the ESC key is pressed

    $(document).keyup(function(event) {
        if (event.keyCode === 27) {
            $("ul.contentsMenu > li > ul").css("visibility", "hidden");
        }
    });

    // Show/hide a given sub-menu

    $("ul.contentsMenu > li > ul > li > div > div > div").click(function(event) {
        if ($(this).hasClass("subMenuOpened")) {
            showContentsSubMenu($(this), false);
        } else if ($(this).hasClass("subMenuClosed")) {
            for (let i = 1; i <= $("ul.contentsMenu > li > ul").attr("subMenus"); ++i) {
                let subMenuButton = $("ul.contentsMenu > li > ul > li > div > div > div#subMenu"+i);

                if (subMenuButton.hasClass("subMenuOpened")) {
                    showContentsSubMenu(subMenuButton, i === subMenuSelected);
                }
            }

            showContentsSubMenu($(this), true);
        }

        event.stopPropagation();
    });
}

function headerAndContentsMenu(pageName, relativePath) {
    doHeaderAndContentsMenu(pageName, relativePath,
                            { "menuItems": [
                                { "level": 0, "label": "Home", "link": "index.html" },
                                { "separator": true },
                                { "level": 0, "label": "OpenCOR tutorial", "link": "https://tutorial-on-cellml-opencor-and-pmr.readthedocs.io/en/latest/index.html", "directLink": true },
                                { "separator": true },
                                { "level": 0, "label": "What is new?", "link": "whatIsNew.html" },
                                { "separator": true },
                                { "level": 0, "label": "User documentation", "link": "user/index.html" },
                                { "level": 0, "label": "Developer documentation", "link": "developer/index.html" },
                                { "separator": true },
                                { "level": 0, "label": "Downloads", "link": "downloads/index.html" },
                                { "level": 1, "label": "Previous snapshots", "link": "downloads/previousSnapshots.html" },
                                { "separator": true },
                                { "level": 0, "label": "Supported platforms", "link": "supportedPlatforms.html" },
                                { "separator": true },
                                { "level": 0, "label": "Licensing", "link": "licensing.html" },
                                { "separator": true },
                                { "level": 0, "label": "Team", "link": "team.html" },
                                { "separator": true },
                                { "level": 0, "label": "Contact us", "link": "contactUs.html" }
                              ]
                            });
}
