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

    // Support for the tracking of emails, downloads and external links in
    // Google Analytics

    if (typeof jQuery !== "undefined") {
        jQuery(document).ready(function($) {
            jQuery("a").on("click", function(event) {
                var el = jQuery(this);
                var href = (typeof(el.attr("href")) !== "undefined")?el.attr("href"):"";
                var track = true;

                if (!href.match(/^javascript:/i)) {
                    var elEv = [];

                    elEv.label = href.replace(/%20/g, " ");
                    elEv.nonInter = false;
                    elEv.loc = href;

                    if (href.match(/^mailto\:/i)) {
                        elEv.category = "Emails";
                        elEv.action = "Click email address";
                        elEv.label = elEv.label.replace(/^mailto\: /, "");
                    } else if (href.match(/\.(exe|zip|tar\.gz|dmg)$/i)) {
                        elEv.category = "Downloads";
                        elEv.action = "Click download file";
                    } else if (    href.match(/^https?\:/i)
                               && !href.match(/opencor\.ws/i)) {
                        elEv.category = "External links";
                        elEv.action = "Click external link";
                        elEv.nonInter = true;
                    } else
                        track = false;

                    if (track) {
                        _gaq.push(["_trackEvent", elEv.category, elEv.action, elEv.label, 0, elEv.nonInter]);

                        if ((el.attr("target") === undefined) || (el.attr("target").toLowerCase() !== "_blank")) {
                            setTimeout(function() { location.href = elEv.loc; }, 400);

                            return false;
                        }
                    }
                }
            });
        });
    }
}

// Context menu

function doHeaderAndContentsMenu(pageName, relativePath, r, g, b, data) {
    // Header

    document.write("<div class=\"header\">");
    document.write("    "+pageName);
    document.write("</div>");
    document.write("");

    // CSS styling for our contents menu

    document.write("<style>");
    document.write("    ul.contentsMenu {");
    document.write("        margin: 0px;");
    document.write("        position: fixed;");
    document.write("        top: 3px;");
    document.write("        right: 3px;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li {");
    document.write("        list-style: none;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul {");
    document.write("        visibility: hidden;");
    document.write("        margin: 0px;");
    document.write("        padding: 0px;");
    document.write("        position: fixed;");
    document.write("        top: 29px;");
    document.write("        right: 0px;");
    document.write("        background: rgb(249, 249, 249);");
    document.write("        border: 2px solid rgb("+r+", "+g+", "+b+");");
    document.write("        border-top-style: none;");
    document.write("        border-radius: 0px 0px 5px 5px;");
    document.write("        box-shadow: 0px 5px 5px -5px rgb(103, 103, 103);");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li {");
    document.write("        margin: 1px;");
    document.write("        font-size: 9pt;");
    document.write("        font-weight: normal;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li span.selectedMenuItem {");
    document.write("        display: block;");
    document.write("        background-color: rgba("+r+", "+g+", "+b+", 0.13);");
    document.write("        color: rgb("+r+", "+g+", "+b+");");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li.lastMenuItem span.selectedMenuItem {");
    document.write("        border-radius: 0px 0px 3px 3px;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li a {");
    document.write("        display: block;");
    document.write("        padding: 1px;");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li a:hover {");
    document.write("        margin: 0px;");
    document.write("        border: 0px;");
    document.write("        background-color: rgba("+r+", "+g+", "+b+", 0.79);");
    document.write("        color: rgb(255, 255, 255);");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li.separator {");
    document.write("        margin: 0px;");
    document.write("        padding: 0px;");
    document.write("        border-top: 1px solid rgb("+r+", "+g+", "+b+");");
    document.write("    }");
    document.write("");
    document.write("    ul.contentsMenu li ul li.lastMenuItem a {");
    document.write("        border-radius: 0px 0px 3px 3px;");
    document.write("    }");
    document.write("</style>");
    document.write("");

    // Contents menu

    document.write("<ul class=\"contentsMenu\">");
    document.write("    <li>");
    document.write("        <img src=\""+relativePath+"/res/pics/oxygen/actions/help-about.png\" width=24 height=24 alt=\"Contents\">");
    document.write("        <ul>");

    var items = data.items;

    for (i = 0; i < items.length; ++i) {
        if (items[i].separator) {
            // We are dealing with a menu separator

            document.write("            <li class=\"separator\"></li>");
        } else {
            // We are dealing with a menu item

            var path = items[i].directLink?items[i].link:relativePath+"/"+items[i].link;
            var indent = "";

            for (j = 0; j < items[i].level; ++j)
                indent += "&nbsp;&nbsp;&nbsp;&nbsp;"

            var currentMenuItem = false;

            if (   (   (typeof items[i].label !== "undefined")
                    && (items[i].label.toLowerCase() === pageName.toLowerCase()))
                || ((pageName === "OpenCOR") && (items[i].label === "Home"))
                || (pageName === items[i].label+" Plugin")) {
                currentMenuItem = true;
            }

            var liClass = (i === items.length-1)?" class=\"lastMenuItem\"":"";

            if (currentMenuItem) {
                document.write("            <li"+liClass+"><span class=\"selectedMenuItem\">"+indent+items[i].label+"</span></li>");
            } else {
                if (   (typeof items[i].link !== "undefined")
                    &&  items[i].link.length) {
                    document.write("            <li"+liClass+"><a href=\""+path+"\">"+indent+items[i].label+"</a></li>");
                } else {
                    document.write("            <li"+liClass+">"+indent+items[i].label+"</li>");
                }
            }
        }
    }

    document.write("        </ul>");
    document.write("    </li>");
    document.write("</ul>");

    // Show/hide our contents menu

    $("ul.contentsMenu > li").mouseenter(function() {
        $("ul.contentsMenu > li > ul").css("visibility", "visible");
    });

    $("ul.contentsMenu > li").mouseleave(function() {
        $("ul.contentsMenu > li > ul").css("visibility", "hidden");
    });

    // Hide our contents menu if the ESC key is pressed when our contents menu
    // is visible

    $(document).keyup(function(event) {
        if (   ($("ul.contentsMenu > li > ul").css("visibility") === "visible")
            && (event.keyCode === 27)) {
            $("ul.contentsMenu > li > ul").css("visibility", "hidden");
        }
    });
}

// Copyright information

function copyright(relativePath) {
    var date = new Date();

    document.write("<div class=\"copyright\">");
    document.write("    <div class=\"left\">");
    document.write("        Copyright 2011-"+date.getFullYear());
    document.write("    </div>");
    document.write("    <div class=\"center\">");
    document.write("        <a class=\"copyright\" href=\"https://twitter.com/TeamOpenCOR/\"><img class=\"twitter\" src=\""+relativePath+"/res/pics/twitter.png\" width=30 height=30 alt=\"Twitter\"></a>");
    document.write("    </div>");
    document.write("    <div class=\"right\">");
    document.write("        <a class=\"copyright\" href=\"http://www.virtualrat.org/\"><img class=\"vpr\" src=\""+relativePath+"/res/pics/vprLogo.png\" width=60 height=18 alt=\"VPR\"></a>");
    document.write("    </div>");
    document.write("</div>");
}
