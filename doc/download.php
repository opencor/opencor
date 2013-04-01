<!DOCTYPE html>
<html>
    <head>
        <title>
            Download
        </title>

        <meta http-equiv="content-type" content="text/html; charset=utf-8"/>

        <link href="res/stylesheet.css" rel="stylesheet" type="text/css"/>

        <script src="3rdparty/jQuery/jquery.js" type="text/javascript"></script>
        <script src="res/common.js" type="text/javascript"></script>
        <script src="res/menu.js" type="text/javascript"></script>
    </head>
    <body>
        <script type="text/javascript">
            headerAndContentsMenu("Download", ".");
        </script>

        <p>
            Please find below the official release of OpenCOR, as well as its latest snapshot.
        </p>

<?php
    // Function to retrieve the formatted size of a file

    function formattedFileSize($fileName) {
        $filePath = $_SERVER["DOCUMENT_ROOT"]."/".$fileName;

        if (file_exists($filePath)) {
            $units = array("B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB");
            $fileSize = filesize($filePath);
            $i = floor(log($fileSize, 1024));

            return ceil($fileSize/pow(1024, $i))." ".$units[$i];
        } else {
            return "???";
        }
    }

    // Various initialisations

    date_default_timezone_set("Europe/London");

    $versions = array(array("Version 0.1", 1, 4, 2013, "0-1-0", true,
                            array(array("Windows", array(".exe"), array(".zip")),
                                  array("Linux", array(".tar.gz", 32), array(".tar.gz", 64)),
                                  array("OS X", array(".dmg"), array(".zip")))),
                      array("Latest snapshot", 21, 3, 2013, "2013-03-21", false,
                            array(array("Windows", array(".exe"), array(".zip")),
                                  array("Linux", array(".tar.gz", 32), array(".tar.gz", 64)),
                                  array("OS X", array(".dmg"), array(".zip")))));

    // Output our various versions

    $versionIndex = 0;

    while ($versionIndex != sizeof($versions)) {
        $version = $versions[$versionIndex];

        // Retrieve some information about the version

        $versionTitle = $version[0];
        $versionDay = $version[1];
        $versionMonth = $version[2];
        $versionYear = $version[3];
        $versionVersion = $version[4];
        $versionRecommended = $version[5];
        $versionFiles = $version[6];

        // Output an empty line to separate the different versions, if needed

        if ($versionIndex) {
?>

<?php
        }

        // Output some general information about the version
?>
        <div class="section">
            <table>
                <tbody>
                    <tr>
                        <td>
                            <?php echo $versionTitle."\n"; ?>
                        </td>
                        <td class="date">
                            (<?php echo date("j F Y", mktime(0, 0, 0, $versionMonth, $versionDay, $versionYear)); ?>)
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>

<?php
        // Output some information about the version files

        $versionClasses = "version";

        if ($versionRecommended)
            $versionClasses .= " recommended";
?>
        <table class="<?php echo $versionClasses; ?>">
            <tbody>
                <tr>
<?php
        // Show n platforms per row
        // Note: for aesthetical reasons (i.e. having top/left separators all
        //       the way through), we have a total of m*n platforms...

        $nbOfPlatformsPerRow = 3;
        $nbOfPlatforms = sizeof($versionFiles);

        while ($nbOfPlatforms % $nbOfPlatformsPerRow)
            ++$nbOfPlatforms;

        // Percentage of the total width to use for each platform

        $platformWidth = 100/$nbOfPlatformsPerRow."%";

        // Go through our different platforms

        $platformIndex = 0;

        while ($platformIndex != $nbOfPlatforms) {
            $platformLocalIndex = $platformIndex % $nbOfPlatformsPerRow;

            // 'Close' the current platform if there is a previous platform

            if ($platformIndex) {
                if (sizeof($platformFiles)) {
?>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
<?php
                }
?>
                    </td>
<?php
            }

            // End the current row of platforms and start a new one if we have
            // just 'closed' the current platform after having listed a row of
            // platforms

            if ($platformIndex && !$platformLocalIndex) {
?>
                </tr>
                <tr>
<?php
            }

            // 'Open' the new platform

            $platformClasses = "platform";

            if ($platformIndex >= $nbOfPlatformsPerRow)
                $platformClasses .= " topSeparator";

            if ($platformLocalIndex)
                $platformClasses .= " leftSeparator";
?>
                    <td class="<?php echo $platformClasses; ?>" style="width: <?php echo $platformWidth; ?>">
<?php
            $platformFiles = $versionFiles[$platformIndex];

            if (sizeof($platformFiles)) {
                $platformName = $platformFiles[0];
?>
                        <table>
                            <tbody>
                                <tr>
                                    <td class="logo">
                                        <img src="res/pics/<?php echo str_replace(" ", "", strtolower($platformName)); ?>.png" width=72 height=72 alt="<?php echo $platformName; ?>">
                                    </td>
                                    <td>
                                        <div>
                                            <?php echo $platformName."\n"; ?>
                                        </div>

                                        <ul>
<?php
                // List the platform files

                $platformFileIndex = 1;

                while ($platformFileIndex != sizeof($platformFiles)) {
                    $platformFile = $platformFiles[$platformFileIndex];

                    // Retrieve some information about the platform file

                    $platformFileExtension = $platformFile[0];
                    $platformFileExtraInfo = $platformFile[1];

                    // Determine the file name, type and extra info, if any

                    $platformFileName = "downloads/OpenCOR-".$versionVersion."-".str_replace(" ", "", $platformName).$platformFileExtraInfo.$platformFileExtension;

                    if (   ($platformFileExtension == ".exe")
                        || ($platformFileExtension == ".dmg"))
                        $platformFileType = "Installer";
                    else if ($platformFileExtension == ".zip")
                        $platformFileType = "ZIP file";
                    else if ($platformFileExtension == ".tar.gz")
                        $platformFileType = "Tarball file";
                    else
                        $platformFileType = "???";

                    if (isset($platformFileExtraInfo))
                        $platformFileExtraInfo = " (".$platformFileExtraInfo."-bit)";
                    else
                        $platformFileExtraInfo = "";

                    // List the file for download
?>
                                            <li><a href="http://www.opencor.ws/<?php echo $platformFileName; ?>"><?php echo $platformFileType; ?></a><?php echo $platformFileExtraInfo; ?> <span class="fileSize">(<?php echo formattedFileSize($platformFileName); ?>)</span></li>
<?php
                    // Go to the next platform file

                    ++$platformFileIndex;
                }
            }

            // Go to the next platform

            ++$platformIndex;
        }

        // 'Close' the last platform

        if (sizeof($platformFiles)) {
?>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
<?php
        }
?>
                    </td>
                </tr>
            </tbody>
        </table>
<?php
        // Go to the next version

        ++$versionIndex;
    }
?>

        <p class="warning">
            Please keep in mind that OpenCOR is <em>only</em> at version 0.1...
        </p>

        <script type="text/javascript">
            copyright();
        </script>
    </body>
</html>
