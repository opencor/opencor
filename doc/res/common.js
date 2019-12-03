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

    $(document).ready(function() {
        $("a").click(function() {
            var element = $(this);
            var href = (typeof element.attr("href") !== "undefined")?element.attr("href"):"";
            var track = true;

            if (!href.match(/^javascript:/i)) {
                var elementEvent = [];

                elementEvent.label = href.replace(/%20/g, " ");
                elementEvent.nonInter = false;
                elementEvent.loc = href;

                if (href.match(/^mailto:/i)) {
                    elementEvent.category = "Emails";
                    elementEvent.action = "Click email address";
                    elementEvent.label = elementEvent.label.replace(/^mailto: /, "");
                } else if (href.match(/\.(exe|zip|tar\.gz|dmg)$/i)) {
                    elementEvent.category = "Downloads";
                    elementEvent.action = "Click download file";
                } else if (    href.match(/^https?:/i)
                           && !href.match(/opencor\.ws/i)) {
                    elementEvent.category = "External links";
                    elementEvent.action = "Click external link";
                    elementEvent.nonInter = true;
                } else {
                    track = false;
                }

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
