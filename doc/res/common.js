// Support for Google Analytics

if (   document.location.protocol !== "file:"
    && document.location.protocol !== "qthelp:"
    && document.location.host !== "localhost"
    && document.location.host !== "127.0.0.1") {
    var _gaq = _gaq || [];

    _gaq.push(["_setAccount", "UA-39516363-1"]);
    _gaq.push(["_trackPageview"]);

    (function() {
        var ga = document.createElement("script");

        ga.type = "text/javascript";
        ga.async = true;
        ga.src = ((document.location.protocol === "https:")?"https://ssl":"http://www")+".google-analytics.com/ga.js";

        var s = document.getElementsByTagName("script")[0];

        s.parentNode.insertBefore(ga, s);
    })();

    // Support the tracking of emails, downloads and external links in Google
    // Analytics

    $(document).ready(function($) {
        $("a").on("click", function() {
            var element = $(this);
            var href = (typeof element.attr("href") !== "undefined")?element.attr("href"):"";
            var track = true;

            if (!href.match(/^javascript:/i)) {
                var elementEvent = [];

                elementEvent.label = href.replace(/%20/g, " ");
                elementEvent.nonInter = false;
                elementEvent.loc = href;

                if (href.match(/^mailto\:/i)) {
                    elementEvent.category = "Emails";
                    elementEvent.action = "Click email address";
                    elementEvent.label = elementEvent.label.replace(/^mailto\: /, "");
                } else if (href.match(/\.(exe|zip|tar\.gz|dmg)$/i)) {
                    elementEvent.category = "Downloads";
                    elementEvent.action = "Click download file";
                } else if (    href.match(/^https?\:/i)
                           && !href.match(/opencor\.ws/i)) {
                    elementEvent.category = "External links";
                    elementEvent.action = "Click external link";
                    elementEvent.nonInter = true;
                } else
                    track = false;

                if (track) {
                    _gaq.push([ "_trackEvent",
                                elementEvent.category,
                                elementEvent.action,
                                elementEvent.label,
                                0,
                                elementEvent.nonInter
                             ]);

                    if (   (element.attr("target") === "undefined")
                        || (element.attr("target").toLowerCase() !== "_blank")) {
                        setTimeout(function() {
                            location.href = elementEvent.loc;
                        }, 400);

                        return false;
                    }
                }
            }
        });
    });
}

// Header and contents menu

function showContentsSubMenu(subMenuButton, showSubMenu) {
    subMenuButton.removeClass(showSubMenu?"subMenuClosed":"subMenuOpened");
    subMenuButton.addClass(showSubMenu?"subMenuOpened":"subMenuClosed");

    $("ul.contentsMenu > li > ul > li.subMenuItem#"+subMenuButton.attr("id")).css("display", showSubMenu?"list-item":"none");
}

function doHeaderAndContentsMenu(pageName, relativePath, r, g, b, data) {
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
    document.write("        border: 2px solid rgb("+r+", "+g+", "+b+");\n");
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
    document.write("        background: rgba("+r+", "+g+", "+b+", 0.79);\n");
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
    document.write("        background: rgba("+r+", "+g+", "+b+", 0.13);\n");
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
    document.write("        border-top: 1px solid rgb("+r+", "+g+", "+b+");\n");
    document.write("    }\n");
    document.write("</style>\n");
    document.write("\n");

    // Contents menu

    document.write("<ul class=\"contentsMenu\">\n");
    document.write("    <li>\n");
    document.write("        <img src=\""+relativePath+"/res/pics/oxygen/actions/help-about.png\" width=24 height=24>\n");
    document.write("        <ul>\n");

    var menuItems = data.menuItems;
    var subMenuCounter = 0;
    var subMenuSelected = 0;

    for (i = 0; i < menuItems.length; ++i) {
        var menuItem = menuItems[i];

        if (menuItem.separator) {
            // We are dealing with a menu separator

            document.write("            <li class=\"separator\"></li>\n");
        } else {
            // We are dealing with a menu item

            var selectedMenuItem = false;

            if (   (   (typeof menuItem.label !== "undefined")
                    && (menuItem.label.toLowerCase() === pageName.toLowerCase()))
                || ((pageName === "OpenCOR") && (menuItem.label === "Home"))
                || (pageName === menuItem.label+" Plugin")) {
                selectedMenuItem = true;
            }

            var liId = "";
            var liClass = "";

            if (   (typeof menuItem.subMenuItem !== "undefined")
                &&  menuItem.subMenuItem) {
                liId = "subMenu"+subMenuCounter;
                liClass = " class=\"subMenuItem\"";
            }

            var menuItemIndent = "";

            for (j = 0; j < menuItem.level; ++j)
                menuItemIndent += "&nbsp;&nbsp;&nbsp;&nbsp;"

            var menuItemLink = "";

            if (   (typeof menuItem.link !== "undefined")
                &&  menuItem.link.length) {
                menuItemLink = menuItem.directLink?menuItem.link:relativePath+"/"+menuItem.link;
            }

            var tableRowClasses = "menuItemTableRow";

            if (i === menuItems.length-1)
                tableRowClasses += " lastMenuItem";

            if (selectedMenuItem) {
                tableRowClasses += " selectedMenuItem";

                if (liId.length)
                    subMenuSelected = subMenuCounter;
            } else {
                if (menuItemLink.length) {
                    tableRowClasses += " clickableMenuItem";
                } else {
                    tableRowClasses += " nonClickableMenuItem";
                }
            }

            var subMenuButton = "";

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

            if (subMenuButton.length)
                document.write("                        "+subMenuButton+"\n");

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

    for (i = 1; i <= subMenuCounter; ++i) {
        var subMenuButton = $("ul.contentsMenu > li > ul > li > div > div > div#subMenu"+i);

        if (i === subMenuSelected)
            subMenuButton.css("display", "none");

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
            for (i = 1; i <= $("ul.contentsMenu > li > ul").attr("subMenus"); ++i)
                showContentsSubMenu($("ul.contentsMenu > li > ul > li > div > div > div#subMenu"+i), i === subMenuSelected);

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
            for (i = 1; i <= $("ul.contentsMenu > li > ul").attr("subMenus"); ++i) {
                var subMenuButton = $("ul.contentsMenu > li > ul > li > div > div > div#subMenu"+i);

                if (subMenuButton.hasClass("subMenuOpened"))
                    showContentsSubMenu(subMenuButton, i === subMenuSelected);
            }

            showContentsSubMenu($(this), true);
        }

        event.stopPropagation();
    });
}

// Return the name of a month

function month(month) {
    switch (month) {
    case 1:
        return "January";
    case 2:
        return "February";
    case 3:
        return "March";
    case 4:
        return "April";
    case 5:
        return "May";
    case 6:
        return "June";
    case 7:
        return "July";
    case 8:
        return "August";
    case 9:
        return "September";
    case 10:
        return "October";
    case 11:
        return "November";
    case 12:
        return "December";
    default:
        return "???";
    }
}

// Render, using KaTeX, all the mathematical equations found in elements that
// 'support' the "katexRendering" class

function katexRendering() {
    var mathematicalEquationsElements = document.getElementsByClassName("katexRendering");

    for (i = 0; i < mathematicalEquationsElements.length; ++i) {
        renderMathInElement(mathematicalEquationsElements[i],
                            {
                                delimiters: [
                                    { left: "$$", right: "$$", display: true },
                                    { left: "\\[", right: "\\]", display: true },
                                    { left: "$", right: "$", display: false },
                                    { left: "\\(", right: "\\)", display: false }
                                ]
                            });
    }
}

// Copyright information

function copyright(relativePath) {
    var date = new Date();

    document.write("<div class=\"copyright\">\n");
    document.write("    <div class=\"left\">\n");
    document.write("        Copyright 2011-"+date.getFullYear()+"\n");
    document.write("    </div>\n");
    document.write("    <div class=\"center\">\n");
    document.write("        <a class=\"copyright\" href=\"https://twitter.com/TeamOpenCOR/\"><img class=\"twitter\" src=\""+relativePath+"/res/pics/twitter.png\" width=30 height=30></a>\n");
    document.write("    </div>\n");
    document.write("    <div class=\"right\">\n");
    document.write("        <a class=\"copyright\" href=\"http://www.virtualrat.org/\"><img class=\"vpr\" src=\""+relativePath+"/res/pics/vpr.png\" width=60 height=18></a>\n");
    document.write("    </div>\n");
    document.write("</div>\n");
}
