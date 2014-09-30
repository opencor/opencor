<?php
    // Check whether we want the list of downloads for the main downloads page
    // or the old snapshots page

    if (!isset($oldSnapshots))
        $oldSnapshots = false;
?>

<!DOCTYPE html>
<html>
    <head>
        <title>
<?php
    if ($oldSnapshots) {
?>
            Old Snapshots
<?php
    } else {
?>
            Downloads
<?php
    }
?>
        </title>

        <meta http-equiv="content-type" content="text/html; charset=utf-8"/>

        <link href="../res/stylesheet.css" rel="stylesheet" type="text/css"/>

        <script src="../3rdparty/jQuery/jquery.js" type="text/javascript"></script>
        <script src="../res/common.js" type="text/javascript"></script>
        <script src="../res/menu.js" type="text/javascript"></script>
    </head>
    <body ondragstart="return false;" ondrop="return false;">
        <script type="text/javascript">
<?php
    if ($oldSnapshots) {
?>
            headerAndContentsMenu("Old Snapshots", "..");
<?php
    } else {
?>
            headerAndContentsMenu("Downloads", "..");
<?php
    }
?>
        </script>

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

    // Output our various versions

    $versionIndex = 0;

    while ($versionIndex != sizeof($versions)) {
        $version = $versions[$versionIndex];

        // Retrieve some information about the version

        $versionMajor = $version[0];
        $versionMinor = $version[1];
        $versionPatch = $version[2];
        $versionDay   = $version[3];
        $versionMonth = $version[4];
        $versionYear  = $version[5];
        $versionType  = $version[6];
        $versionFiles = $version[7];
        $versionInfo  = $version[8];

        if ($versionMajor || $versionMinor || $versionPatch) {
            $versionTitle   = "Version ".$versionMajor.".".$versionMinor;
            $versionVer     = $versionMajor.".".$versionMinor;
            $versionVersion = $versionMajor."-".$versionMinor;

            if ($versionPatch) {
                $versionTitle   .= ".".$versionPatch;
                $versionVer     .= ".".$versionPatch;
                $versionVersion .= "-".$versionPatch;
            }
        } else {
            if ($oldSnapshots)
                $versionTitle = date("j F Y", mktime(0, 0, 0, $versionMonth, $versionDay, $versionYear));
            else
                $versionTitle = "Latest snapshot";

            $versionVer = "snapshots/".date("Y-m-d", mktime(0, 0, 0, $versionMonth, $versionDay, $versionYear));
            $versionVersion = date("Y-m-d", mktime(0, 0, 0, $versionMonth, $versionDay, $versionYear));
        }

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
<?php
        if (!$oldSnapshots) {
?>
                            <span class="whatIsNew"><a href="../user/whatIsNew.html#<?php echo $versionVer; ?>">What is new?</a></span>
<?php
        }
?>
                        </td>
                        <td class="date">
<?php
        if (!$oldSnapshots) {
?>
                            (<?php echo date("j F Y", mktime(0, 0, 0, $versionMonth, $versionDay, $versionYear)); ?>)
<?php
        }
?>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>

<?php
        // Output some information about the version files

        $versionClass = "";

        if ($versionType == 1)
            $versionClass = "official";
        else if (($versionType == 2) && !$oldSnapshots)
            $versionClass = "latest";

        if (strlen($versionInfo))
            $versionClass .= " withInfo";
        else
            $versionClass .= " withoutInfo";
?>
        <div class="<?php echo $versionClass; ?>">
            <table class="version">
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
                $platformSupported = $platformFiles[1];
?>
                            <table>
                                <tbody>
                                    <tr>
                                        <td class="logo">
                                            <img src="../res/pics/<?php echo str_replace(" ", "", strtolower($platformName)); ?>.png" width=72 height=72 alt="<?php echo $platformName; ?>">
                                        </td>
                                        <td>
                                            <div class="name">
                                                <?php echo $platformName."\n"; ?>
                                            </div>

                                            <div class="supported">
                                                <?php echo $platformSupported."\n"; ?>
                                            </div>

                                            <ul>
<?php
                // List the platform files

                $platformFileIndex = 2;

                while ($platformFileIndex != sizeof($platformFiles)) {
                    $platformFile = $platformFiles[$platformFileIndex];

                    // Retrieve some information about the platform file

                    $platformFileExtension = $platformFile[0];
                    $platformFileExtraInfo = $platformFile[1];

                    // Determine the file name, type and extra info, if any

                    $platformFileName = "downloads/".$versionVer."/OpenCOR-".$versionVersion."-".str_replace(" ", "", $platformName).$platformFileExtraInfo.$platformFileExtension;

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
        </div>
<?php
        // Add some information, if needed

        if (strlen($versionInfo)) {
            if ($versionType == 1)
                $versionClass = "officialInfo";
            else if ($versionType == 2)
                $versionClass = "latestInfo";
?>
        <div class="<?php echo $versionClass; ?>">
<?php
            echo $versionInfo;
?>
        </div>
<?php
        }

        // Go to the next version

        ++$versionIndex;
    }

    if (!$oldSnapshots) {
?>
        <p class="note">
            old snapshots can be found <a href="oldSnapshots.php">here</a>.
        </p>
<?php
    }
?>

        <script type="text/javascript">
            copyright("..");
        </script>
    </body>
</html>
