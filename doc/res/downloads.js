function twoDigits(number) {
    if (number < 10)
        return "0"+number;
    else
        return number;
}

function fileSize(size) {
    if (typeof size !== "undefined") {
        var units = [ "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" ];
        var index = Math.floor(Math.log(size)/Math.log(1024));

        return Math.ceil(size/Math.pow(1024, index))+" "+units[index];
    } else {
        return "???";
    }
}

function hasVersions() {
    return jsonData.versions.length;
}

function hasSnapshots() {
    var versions = jsonData.versions;

    if (versions.length) {
        for (var versionIndex = 0; versionIndex < versions.length; ++versionIndex) {
            if (versions[versionIndex].type === 2)
                return true;
        }

        return false;
    } else {
        return false;
    }
}

function versions(downloads) {
    // Make sure that the jsonData variable is available

    if (typeof jsonData === "undefined")
        return;

    // Retrieve the size of our various downloadable files, should we be
    // running this script from opencor.ws
    // Note: the URL used in the request must be based on the value of
    //       location.hostname rather than on www.opencor.ws or opencor.ws.
    //       Indeed, if the user was to use www.opencor.ws to look up a page
    //       and we were to use opencor.ws for our request, then the request
    //       would fail because we don't use the exact same hostname...

    var fileSizes = {};

    if (location.hostname.substring(location.hostname.length-10) === "opencor.ws") {
        var fileSizesData;

        $.ajax({ url: "http://"+location.hostname+"/res/downloads.php", async: false,
                 success: function(data) { fileSizesData = data; } });

        var retrievedFileSizes = fileSizesData.split("\n");

        for (var i = 0, iMax = retrievedFileSizes.length; i < iMax; ++i) {
            var retrievedFileSize = retrievedFileSizes[i].split(",");

            fileSizes[retrievedFileSize[0]] = retrievedFileSize[1];
        }
    }

    // Output the various requested downloadable files, using the information
    // contained in the given JSON file

    var versions = jsonData.versions;

    if (versions.length) {
        // Some CSS for our downloads menus

        document.write("<style>\n");
        document.write("    span.fakeLink {\n");
        document.write("        color: rgb(103, 103, 103);\n");
        document.write("        cursor: pointer;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    span.fakeLink:hover {\n");
        document.write("        margin: -1px;\n");
        document.write("        border: 1px solid rgba(103, 103, 103, 0.13);\n");
        document.write("        background: rgba(103, 103, 103, 0.05);\n");
        document.write("        color: rgb(103, 103, 103);\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    table ul.downloadsMenu {\n");
        document.write("        border-spacing: 0px;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu {\n");
        document.write("        z-index: 999;\n");
        document.write("        -khtml-user-select: none;\n");
        document.write("        -moz-user-select: none;\n");
        document.write("        -ms-user-select: none;\n");
        document.write("        -webkit-touch-callout: none;\n");
        document.write("        -webkit-user-select: none;\n");
        document.write("        user-select: none;\n");
        document.write("        position: absolute;\n");
        document.write("        padding: 0px;\n");
        document.write("        visibility: hidden;\n");
        document.write("        background: rgb(249, 249, 249);\n");
        document.write("        border: 1px solid rgb(103, 103, 103);\n");
        document.write("        border-radius: 5px;\n");
        document.write("        box-shadow: 0px 5px 5px -5px rgb(103, 103, 103);\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li {\n");
        document.write("        list-style: none;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li {\n");
        document.write("        cursor: default;\n");
        document.write("        margin: 1px;\n");
        document.write("        font-size: 8pt;\n");
        document.write("        font-weight: normal;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li > div > div > div:hover {\n");
        document.write("        background: rgba(103, 103, 103, 0.79);\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li div:hover a {\n");
        document.write("        color: rgb(255, 255, 255);\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li div.firstMenuItem > div:hover {\n");
        document.write("        border-radius: 3px 3px 0px 0px;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li div.lastMenuItem > div:hover {\n");
        document.write("        border-radius: 0px 0px 3px 3px;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li div.menuItemLabel {\n");
        document.write("        display: table-cell;\n");
        document.write("        font-style: normal;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li div.menuItemTable {\n");
        document.write("        display: table;\n");
        document.write("        width: 100%;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu > li div.menuItemTableRow {\n");
        document.write("        display: table-row;\n");
        document.write("        vertical-align: middle;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu li a {\n");
        document.write("        display: block;\n");
        document.write("        padding: 1px;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("    ul.downloadsMenu li a:hover {\n");
        document.write("        margin: 0px;\n");
        document.write("        border: 0px;\n");
        document.write("    }\n");
        document.write("\n");
        document.write("</style>\n");
        document.write("\n");

        // Now, output the various requested downloadable files

        var idCounter = 0;

        for (var versionIndex = 0; versionIndex < versions.length; ++versionIndex) {
            var version = versions[versionIndex];

            // Retrieve some information about the version

            var versionMajor = version.major;
            var versionMinor = version.minor;
            var versionPatch = version.patch;
            var versionDay = version.day;
            var versionMonth = version.month;
            var versionYear = version.year;
            var versionType = version.type;
            var versionPlatforms = version.platforms;
            var versionChanges = version.changes;

            if (versionMajor || versionMinor || versionPatch) {
                versionTitle = "Version "+versionMajor+"."+versionMinor;
                versionFolder = versionMajor+"."+versionMinor;
                versionVersion = versionMajor+"-"+versionMinor;

                if (versionPatch) {
                    versionTitle += "."+versionPatch;
                    versionFolder += "."+versionPatch;
                    versionVersion += "-"+versionPatch;
                }

                versionAnchor = versionFolder;
            } else {
                if (downloads)
                    versionTitle = "Latest snapshot";
                else
                    versionTitle = versionDay+" "+month(versionMonth)+" "+versionYear;

                versionFolder = "snapshots/"+versionYear+"-"+twoDigits(versionMonth)+"-"+twoDigits(versionDay);
                versionVersion = versionYear+"-"+twoDigits(versionMonth)+"-"+twoDigits(versionDay);
                versionAnchor = "latest";
            }

            // Output some general information about the version

            var sectionClass = "section";

            if (versionType === 1)
                sectionClass += " official officialSection";
            else if ((versionType === 2) && downloads)
                sectionClass += " latest latestSection";
            else
                sectionClass += " old oldSection";

            document.write("<div class=\""+sectionClass+"\">\n");
            document.write("    <table>\n");
            document.write("        <tbody>\n");
            document.write("            <tr>\n");
            document.write("                <td>\n");
            document.write("                    "+versionTitle+"\n");

            if (downloads)
                document.write("                    <span class=\"whatIsNew\"><a href=\"../user/whatIsNew.html#"+versionAnchor+"\">What is new?</a></span>\n");

            document.write("                </td>\n");
            document.write("                <td class=\"date\">\n");

            if (downloads) {
                if (versionDay && versionMonth && versionYear)
                    document.write("                    ("+versionDay+" "+month(versionMonth)+" "+versionYear+")\n");
                else
                    document.write("                    (???)\n");
            }

            document.write("                </td>\n");
            document.write("            </tr>\n");
            document.write("        </tbody>\n");
            document.write("    </table>\n");
            document.write("</div>\n");

            // Output some information about the version files

            var versionClass;

            if (versionType === 1)
                versionClass = "official";
            else if ((versionType === 2) && downloads)
                versionClass = "latest";
            else
                versionClass = "old";

            document.write("<div class=\"contents "+versionClass+"\">\n");
            document.write("    <table class=\"version\">\n");
            document.write("        <tbody>\n");
            document.write("            <tr>\n");

            // Show n platforms per row
            // Note: for aesthetical reasons (i.e. having top/left separators all
            //       the way through), we have a total of m*n platforms...

            var nbOfPlatformsPerRow = 3;
            var nbOfPlatforms = versionPlatforms.length;

            while (nbOfPlatforms % nbOfPlatformsPerRow)
                ++nbOfPlatforms;

            // Percentage of the total width to use for each platform

            var platformWidth = 100/nbOfPlatformsPerRow+"%";

            // Go through our different platforms

            for (var platformIndex = 0; platformIndex < nbOfPlatforms; ++platformIndex) {
                var platformLocalIndex = platformIndex % nbOfPlatformsPerRow;

                // 'Close' the current platform if there is a previous platform

                if (platformIndex) {
                    if (typeof platform !== "undefined") {
                        document.write("                                    </ul>\n");
                        document.write("                                </td>\n");
                        document.write("                            </tr>\n");
                        document.write("                        </tbody>\n");
                        document.write("                    </table>\n");
                    }

                    document.write("                </td>\n");
                }

                // End the current row of platforms and start a new one if we have
                // just 'closed' the current platform after having listed a row of
                // platforms

                if (platformIndex && !platformLocalIndex) {
                    document.write("            </tr>\n");
                    document.write("            <tr>\n");
                }

                // 'Open' the new platform

                var platformClasses = "platform";

                if (platformIndex >= nbOfPlatformsPerRow)
                    platformClasses += " topSeparator";

                if (platformLocalIndex)
                    platformClasses += " leftSeparator";

                document.write("                <td class=\""+platformClasses+"\" style=\"width: "+platformWidth+"\">\n");

                var platform = versionPlatforms[platformIndex];

                if (typeof platform !== "undefined") {
                    var platformName = platform.name;
                    var platformSupported = platform.supported;
                    var platformFiles = platform.files;

                    document.write("                    <table>\n");
                    document.write("                        <tbody>\n");
                    document.write("                            <tr>\n");
                    document.write("                                <td class=\"logo\">\n");
                    document.write("                                    <img src=\"../res/pics/"+platformName.replace(" ", "").toLowerCase()+".png\" width=72 height=72>\n");
                    document.write("                                </td>\n");
                    document.write("                                <td>\n");
                    document.write("                                    <div class=\"name\">\n");
                    document.write("                                        "+platformName+"\n");
                    document.write("                                    </div>\n");
                    document.write("                                    <div class=\"supported\">\n");
                    document.write("                                        "+platformSupported+"\n");
                    document.write("                                    </div>\n");
                    document.write("                                    <ul>\n");

                    // List the files

                    for (var fileIndex = 0; fileIndex < platformFiles.length; ++fileIndex) {
                        var file = platformFiles[fileIndex];

                        // Retrieve some information about the file

                        var fileExtension = file.extension;
                        var fileBitness = file.bitness;

                        // Determine the file name, type and extra info, if any

                        var fileName = "OpenCOR-"+versionVersion+"-"+platformName.replace(" ", "")+((typeof fileBitness !== "undefined")?fileBitness:"")+fileExtension;
                        var fullFileName = versionFolder+"/"+fileName;

                        if (   (fileExtension === ".exe")
                            || (fileExtension === ".dmg"))
                            fileType = "Installer";
                        else if (fileExtension === ".zip")
                            fileType = "ZIP file";
                        else if (fileExtension === ".tar.gz")
                            fileType = "Tarball file";
                        else
                            fileType = "???";

                        if (typeof fileBitness !== "undefined")
                            fileBitness = " ("+fileBitness+"-bit)";
                        else
                            fileBitness = "";

                        // List the file for download

                        if (versionType !== 2) {
                            document.write("                                        <li>\n");
                            document.write("                                            <span class=\"fakeLink\" id=\"id"+(++idCounter)+"\">"+fileType+"</span>"+fileBitness+" <span class=\"fileSize\">("+fileSize(fileSizes[fullFileName])+")</span>\n");
                            document.write("                                            <ul class=\"downloadsMenu\" id=\"id"+idCounter+"\">\n");
                            document.write("                                                <li>\n");
                            document.write("                                                    <div class=\"menuItemTable\">\n");
                            document.write("                                                        <div class=\"menuItemTableRow firstMenuItem\">\n");
                            document.write("                                                            <div class=\"menuItemLabel\">\n");
                            document.write("                                                                <a href=\""+fullFileName+"\">Download from here</a>\n");
                            document.write("                                                            </div>\n");
                            document.write("                                                        </div>\n");
                            document.write("                                                    </div>\n");
                            document.write("                                                </li>\n");
                            document.write("                                                <li>\n");
                            document.write("                                                    <div class=\"menuItemTable\">\n");
                            document.write("                                                        <div class=\"menuItemTableRow lastMenuItem\">\n");
                            document.write("                                                            <div class=\"menuItemLabel\">\n");
                            document.write("                                                                <a href=\"https://github.com/opencor/opencor/releases/download/v"+versionAnchor+"/"+fileName+"\">Download from GitHub</a>\n");
                            document.write("                                                            </div>\n");
                            document.write("                                                        </div>\n");
                            document.write("                                                    </div>\n");
                            document.write("                                                </li>\n");
                            document.write("                                            </ul>\n");
                            document.write("                                        </li>\n");

                            // Show our downloads menu (and hide all the other
                            // ones) if we click on our span element

                            $("span#id"+idCounter).click(function(event) {
                                $("ul.downloadsMenu").css("visibility", "hidden");
                                $("ul#"+$(this).attr("id")).css("visibility", "visible");

                                event.stopPropagation();
                            });
                        } else {
                            document.write("                                        <li><a href=\""+fullFileName+"\">"+fileType+"</a>"+fileBitness+" <span class=\"fileSize\">("+fileSize(fileSizes[fullFileName])+")</span></li>\n");
                        }
                    }
                }
            }

            // 'Close' the last platform

            if (typeof platform !== "undefined") {
                document.write("                                    </ul>\n");
                document.write("                                </td>\n");
                document.write("                            </tr>\n");
                document.write("                        </tbody>\n");
                document.write("                    </table>\n");
            }

            document.write("                </td>\n");
            document.write("            </tr>\n");
            document.write("        </tbody>\n");
            document.write("    </table>\n");

            // Add the changes, if any

            if (typeof versionChanges !== "undefined") {
                if (versionChanges.length) {
                    document.write("    <table class=\"changes\">\n");
                    document.write("        <tbody>\n");
                    document.write("            <tr>\n");
                    document.write("                <td class=\"topSeparator\">\n");
                    document.write("                    <ul>\n");

                    for (var changeIndex = 0; changeIndex < versionChanges.length; ++changeIndex)
                        document.write("                        <li><span class=\"changes\">"+versionChanges[changeIndex].change+"</span></li>");

                    document.write("                    </ul>\n");
                    document.write("                </td>\n");
                    document.write("            </tr>\n");
                    document.write("        </tbody>\n");
                    document.write("    </table>\n");
                }
            }

            document.write("</div>\n");
        }

        // Hide our downloads menus if the ESC key is pressed or if we press
        // anywhere on the document

        $(document).keyup(function(event) {
            if (event.keyCode === 27) {
                $("ul.downloadsMenu").css("visibility", "hidden");
            }
        });

        $(document).click(function() {
            $("ul.downloadsMenu").css("visibility", "hidden");
        });
    } else {
        document.write("<p>\n");
        document.write("    No "+(downloads?"downloads":"previous snapshots")+" are available.\n");
        document.write("</p>\n");
    }
}
