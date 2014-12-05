function twoDigits(number) {
    if (number < 10)
        return "0"+number;
    else
        return number;
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

function fileSize(size) {
    if (typeof size !== "undefined") {
        var units = [ "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" ];
        var index = Math.floor(Math.log(size)/Math.log(1024));

        return Math.ceil(size/Math.pow(1024, index))+" "+units[index];
    } else {
        return "???";
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

    for (var versionIndex = 0; versionIndex < versions.length; ++versionIndex) {
        var version = versions[versionIndex];

        // Retrieve some information about the version

        var versionMajor       = version.major;
        var versionMinor       = version.minor;
        var versionPatch       = version.patch;
        var versionDay         = version.day;
        var versionMonth       = version.month;
        var versionYear        = version.year;
        var versionType        = version.type;
        var versionPlatforms   = version.platforms;
        var versionInformation = version.information;

        if (versionMajor || versionMinor || versionPatch) {
            versionTitle   = "Version "+versionMajor+"."+versionMinor;
            versionFolder  = versionMajor+"."+versionMinor;
            versionVersion = versionMajor+"-"+versionMinor;

            if (versionPatch) {
                versionTitle   += "."+versionPatch;
                versionFolder  += "."+versionPatch;
                versionVersion += "-"+versionPatch;
            }
        } else {
            if (downloads)
                versionTitle = "Latest snapshot";
            else
                versionTitle = versionDay+" "+month(versionMonth)+" "+versionYear;

            versionFolder = "snapshots/"+versionYear+"-"+twoDigits(versionMonth)+"-"+twoDigits(versionDay);
            versionVersion = versionYear+"-"+twoDigits(versionMonth)+"-"+twoDigits(versionDay);
        }

        // Output some general information about the version

        document.write("<div class=\"section\">\n");
        document.write("    <table>\n");
        document.write("        <tbody>\n");
        document.write("            <tr>\n");
        document.write("                <td>\n");
        document.write("                    "+versionTitle+"\n");

        if (downloads)
            document.write("                    <span class=\"whatIsNew\"><a href=\"../user/whatIsNew.html#"+versionFolder+"\">What is new?</a></span>\n");

        document.write("                </td>\n");
        document.write("                <td class=\"date\">\n");

        if (downloads)
            document.write("                    ("+versionDay+" "+month(versionMonth)+" "+versionYear+")\n");

        document.write("                </td>\n");
        document.write("            </tr>\n");
        document.write("        </tbody>\n");
        document.write("    </table>\n");
        document.write("</div>\n");

        // Output some information about the version files

        var versionClass = "";

        if (versionType === 1)
            versionClass = "official";
        else if ((versionType === 2) && downloads)
            versionClass = "latest";

        if (typeof versionInformation !== "undefined")
            versionClass += " withInfo";
        else
            versionClass += " withoutInfo";

        document.write("<div class=\""+versionClass+"\">\n");
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

            document.write("            <td class=\""+platformClasses+"\" style=\"width: "+platformWidth+"\">\n");

            var platform = versionPlatforms[platformIndex];

            if (typeof platform !== "undefined") {
                var platformName      = platform.name;
                var platformSupported = platform.supported;
                var platformFiles     = platform.files;

                document.write("            <table>\n");
                document.write("                <tbody>\n");
                document.write("                    <tr>\n");
                document.write("                        <td class=\"logo\">\n");
                document.write("                            <img src=\"../res/pics/"+platformName.replace(" ", "").toLowerCase()+".png\" width=72 height=72 alt=\""+platformName+"\">\n");
                document.write("                        </td>\n");
                document.write("                        <td>\n");
                document.write("                            <div class=\"name\">\n");
                document.write("                                "+platformName+"\n");
                document.write("                            </div>\n");
                document.write("                            <div class=\"supported\">\n");
                document.write("                                "+platformSupported+"\n");
                document.write("                            </div>\n");
                document.write("                            <ul>\n");

                // List the files

                for (var fileIndex = 0; fileIndex < platformFiles.length; ++fileIndex) {
                    var file = platformFiles[fileIndex];

                    // Retrieve some information about the file

                    var fileExtension = file.extension;
                    var fileBitness   = file.bitness;

                    // Determine the file name, type and extra info, if any

                    var fileName = versionFolder+"/OpenCOR-"+versionVersion+"-"+platformName.replace(" ", "")+((typeof fileBitness !== "undefined")?fileBitness:"")+fileExtension;

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

                    document.write("                            <li><a href=\""+fileName+"\">"+fileType+"</a>"+fileBitness+" <span class=\"fileSize\">("+fileSize(fileSizes[fileName])+")</span></li>\n");
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
        document.write("</div>\n");

        // Add some information, if needed

        if (typeof versionInformation !== "undefined") {
            var versionClass;

            if (versionType === 1)
                versionClass = "officialInfo";
            else if (versionType === 2)
                versionClass = "latestInfo";

            document.write("<div class=\""+versionClass+"\">\n");
            document.write("    "+versionInformation+"\n");
            document.write("</div>\n");
        }
    }
}
