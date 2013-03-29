<!DOCTYPE html>
<?php
    // Retrieve the formatted size of a file

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
?>
<html>
    <head>
        <title>
            Downloads
        </title>

        <meta http-equiv="content-type" content="text/html; charset=utf-8"/>

        <link href="res/stylesheet.css" rel="stylesheet" type="text/css"/>

        <script src="3rdparty/jQuery/jquery.js" type="text/javascript"></script>
        <script src="res/common.js" type="text/javascript"></script>
        <script src="res/menu.js" type="text/javascript"></script>
    </head>
    <body>
        <script type="text/javascript">
            headerAndContentsMenu("Downloads", ".");
        </script>

        <p>
            Please find below the latest snapshot available.
        </p>

<?php
    date_default_timezone_set("Europe/Paris");

    $downloads = array(
                     array("Latest snapshot", 21, 3, 2013, "2013-03-21", false,
                           array(array("Windows", ".exe"), array("Windows", ".zip"),
                                 array("Linux", ".tar.gz", 32), array("Linux", ".tar.gz", 64),
                                 array("OSX", ".dmg"), array("OSX", ".zip"))),
                 );

    foreach ($downloads as $download) {
?>
        <div class="section">
            <table>
                <tbody>
                    <tr>
                        <td>
                            <?php echo $download[0]."\n"; ?>
                        </td>
                        <td class="date">
                            (<?php echo date("j F Y", mktime(0, 0, 0, $download[2], $download[1], $download[3])); ?>)
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>

<?php
        if ($download[5]) {
?>
        <table class="downloads recommended">
<?php
        } else {
?>
        <table class="downloads">
<?php
        }
?>
            <tbody>
                <tr>
<?php
        $platform = "";

        foreach ($download[6] as $file) {
            if ($file[0] != $platform) {
                // We are dealing with a file for a new platform, so check
                // whether we need to 'close' the previous platform (and add a
                // separator) before opening the new one

                if ($platform != "") {
                    // There is a previous platform, so close it and add a
                    // separator
?>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                    <td class="separator" />
<?php
                }

                // Keep track of the new platform

                $platform = $file[0];

                // Open the new platform
?>
                    <td class="platform">
                        <table>
                            <tbody>
                                <tr>
                                    <td class="logo">
                                        <img src="res/pics/<?php echo strtolower($platform); ?>.png" width=72 height=72 alt="<?php echo $platform; ?>">
                                    </td>
                                    <td>
                                        <div>
                                            <?php echo $platform."\n"; ?>
                                        </div>

                                        <ul>
<?php
            }

            // Determine the file name

            $fileName = "downloads/OpenCOR-".$download[4]."-".$file[0];

            if (isset($file[2]))
                $fileName .= $file[2];

            $fileName .= $file[1];

            // Determine the file type

            $fileType = "???";

            if (($file[1] == ".exe") || ($file[1] == ".dmg"))
                $fileType = "Installer";
            else if ($file[1] == ".zip")
                $fileType = "ZIP file";
            else if ($file[1] == ".tar.gz")
                $fileType = "Tarball file";

            // Determine the file extra info, if any

            $fileExtraInfo = "";

            if (isset($file[2]))
                $fileExtraInfo = " (".$file[2]."-bit)";

            // List the file for download
?>
                                            <li><a href="http://www.opencor.ws/<?php echo $fileName; ?>"><?php echo $fileType; ?></a><?php echo $fileExtraInfo; ?> <span class="fileSize">(<?php echo formattedFileSize($fileName); ?>)</span></li>
<?php
        }
?>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </tbody>
        </table>
<?php
    }
?>

        <script type="text/javascript">
            copyright();
        </script>
    </body>
</html>
