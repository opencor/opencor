function twoDigits(number) {
    if (number < 10) {
        return "0"+number;
    } else {
        return number;
    }
}

function fileSize(size) {
    if (typeof size !== "undefined") {
        let units = [ "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" ];
        let index = Math.floor(Math.log(size)/Math.log(1024));

        return Math.ceil(size/Math.pow(1024, index))+" "+units[index];
    } else {
        return "???";
    }
}

function versions(downloads) {
    // Make sure that the jsonData variable is available

    if (typeof jsonData === "undefined") {
        return;
    }

    // Retrieve the size of our various downloadable files, should we be
    // running this script from opencor.ws
    // Note: the URL used in the request must be based on the value of
    //       location.hostname rather than on www.opencor.ws or opencor.ws.
    //       Indeed, if the user was to use www.opencor.ws to look up a page
    //       and we were to use opencor.ws for our request, then the request
    //       would fail because we don't use the exact same hostname...

    let fileSizes = {};

    if (location.hostname.indexOf("opencor") !== -1) {
        let fileSizesData;

        $.ajax({ url: location.protocol+"//"+location.hostname+"/res/downloads.php", async: false,
                 success: function(data) { fileSizesData = data; } });

        let retrievedFileSizes = fileSizesData.split("\n");

        for (let i = 0, iMax = retrievedFileSizes.length; i < iMax; ++i) {
            let retrievedFileSize = retrievedFileSizes[i].split(",");

            fileSizes[retrievedFileSize[0]] = retrievedFileSize[1];
        }
    }

    // Output the various requested downloadable files, using the information
    // contained in the given JSON file

    let versions = jsonData.versions;

    if (versions.length) {
        // Now, output the various requested downloadable files

        let idCounter = 0;

        for (let versionIndex = 0; versionIndex < versions.length; ++versionIndex) {
            let version = versions[versionIndex];

            // Retrieve some information about the version

            let versionMajor = version.major;
            let versionMinor = version.minor;
            let versionPatch = version.patch;
            let versionDay = version.day;
            let versionMonth = version.month;
            let versionYear = version.year;
            let versionType = version.type;
            let versionLicense = version.license;
            let versionPlatforms = version.platforms;
            let versionChanges = version.changes;

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
                if (downloads) {
                    versionTitle = "Latest snapshot";
                } else {
                    versionTitle = versionDay+" "+month(versionMonth)+" "+versionYear;
                }

                versionFolder = "snapshots/"+versionYear+"-"+twoDigits(versionMonth)+"-"+twoDigits(versionDay);
                versionVersion = versionYear+"-"+twoDigits(versionMonth)+"-"+twoDigits(versionDay);
                versionAnchor = "latest";
            }

            // Output some general information about the version

            let downloadClass = "download";

            if (versionType === 1) {
                downloadClass += " official officialDownload";
            } else if ((versionType === 2) && downloads) {
                downloadClass += " latest latestDownload";
            } else {
                downloadClass += " old oldDownload";
            }

            document.write("<div class=\""+downloadClass+"\">\n");
            document.write("    <table>\n");
            document.write("        <tbody>\n");
            document.write("            <tr>\n");
            document.write("                <td>\n");
            document.write("                    "+versionTitle+"\n");

            if (versionLicense == 1) {
                document.write("                    <span class=\"license\"><a href=\"https://opensource.org/licenses/Apache-2.0\">Apache v2.0</a></span>\n");
            } else if (versionLicense == 2) {
                document.write("                    <span class=\"license\"><a href=\"https://opensource.org/licenses/GPL-3.0\">GPL v3.0</a></span>\n");
            }

            document.write("                </td>\n");

            if (downloads) {
                document.write("                <td class=\"whatIsNew\">\n");
                document.write("                    <span class=\"whatIsNew\"><a href=\"../whatIsNew.html#"+versionAnchor+"\">What is new?</a></span>\n");
                document.write("                </td>\n");
            }

            document.write("                <td class=\"date\">\n");

            if (downloads) {
                if (versionDay && versionMonth && versionYear) {
                    document.write("                    "+versionDay+" "+month(versionMonth)+" "+versionYear+"\n");
                } else {
                    document.write("                    (???)\n");
                }
            }

            document.write("                </td>\n");
            document.write("            </tr>\n");
            document.write("        </tbody>\n");
            document.write("    </table>\n");
            document.write("</div>\n");

            // Output some information about the version files

            let versionClass;

            if (versionType === 1) {
                versionClass = "official";
            } else if ((versionType === 2) && downloads) {
                versionClass = "latest";
            } else {
                versionClass = "old";
            }

            document.write("<div class=\"contents "+versionClass+"\">\n");
            document.write("    <table class=\"version\">\n");
            document.write("        <tbody>\n");
            document.write("            <tr>\n");

            // Show n platforms per row
            // Note: for aesthetical reasons (i.e. having top/left separators all
            //       the way through), we have a total of m*n platforms...

            let nbOfPlatformsPerRow = 3;
            let nbOfPlatforms = versionPlatforms.length;

            while (nbOfPlatforms % nbOfPlatformsPerRow) {
                ++nbOfPlatforms;
            }

            // Percentage of the total width to use for each platform

            let platformWidth = 100/nbOfPlatformsPerRow+"%";

            // Go through our different platforms

            let platform = null;

            for (let platformIndex = 0; platformIndex < nbOfPlatforms; ++platformIndex) {
                let platformLocalIndex = platformIndex % nbOfPlatformsPerRow;

                // 'Close' the current platform if there is a previous platform

                if (platformIndex) {
                    if (platform !== null) {
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

                let platformClasses = "platform";

                if (platformIndex >= nbOfPlatformsPerRow) {
                    platformClasses += " topSeparator";
                }

                if (platformLocalIndex) {
                    platformClasses += " leftSeparator";
                }

                document.write("                <td class=\""+platformClasses+"\" style=\"width: "+platformWidth+"\">\n");

                platform = versionPlatforms[platformIndex];

                if (platform !== null) {
                    let platformName = platform.name;
                    let platformSupported = platform.supported;
                    let platformFiles = platform.files;

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

                    for (let fileIndex = 0; fileIndex < platformFiles.length; ++fileIndex) {
                        let file = platformFiles[fileIndex];

                        // Retrieve some information about the file

                        let fileExtension = file.extension;
                        let fileBitness = file.bitness;

                        // Determine the file name, type and extra info, if any

                        let fileName = "OpenCOR-"+versionVersion+"-"+platformName.replace(" ", "")+((typeof fileBitness !== "undefined")?fileBitness:"")+fileExtension;
                        let fullFileName = versionFolder+"/"+fileName;
                        let fileType;

                        if (   (fileExtension === ".exe")
                            || (fileExtension === ".dmg")
                            || (fileExtension === ".pkg")) {
                            fileType = "Installer";
                        } else if (fileExtension === ".zip") {
                            fileType = "ZIP file";
                        } else if (fileExtension === ".tar.gz") {
                            fileType = "Tarball file";
                        } else {
                            fileType = "???";
                        }

                        if (typeof fileBitness !== "undefined") {
                            fileBitness = " ("+fileBitness+"-bit)";
                        } else {
                            fileBitness = "";
                        }

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

            if (platform !== null) {
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

                    for (let changeIndex = 0; changeIndex < versionChanges.length; ++changeIndex) {
                        document.write("                        <li><span class=\"changes\">"+versionChanges[changeIndex].change+"</span></li>");
                    }

                    document.write("                    </ul>\n");
                    document.write("                </td>\n");
                    document.write("            </tr>\n");
                    document.write("        </tbody>\n");
                    document.write("    </table>\n");
                }
            }

            document.write("</div>\n");

            // Note about the documentation for the latest snapshot

            if ((versionType === 2) && downloads) {
                document.write("<div class=\"latest latestDownload note\">\n");
                document.write("    <table class=\"note\">\n");
                document.write("        <tbody>\n");
                document.write("            <tr>\n");
                document.write("                <td>\n");
                document.write("                    <span class=\"note\">Note:</span> the user documentation for the latest snapshot can be found <a href=\"https://opencor.github.io/\">here</a>.\n");
                document.write("                </td>\n");
                document.write("            </tr>\n");
                document.write("        </tbody>\n");
                document.write("    </table>\n");
                document.write("</div>\n");
            }
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
        document.write("    No "+(downloads?"downloads":"previous snapshots")+" available.\n");
        document.write("</p>\n");
    }
}
