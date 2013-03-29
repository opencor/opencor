<!DOCTYPE html>
<?php
    // Get the size of a file

    function formattedFileSize($fileName) {
        $filePath = $_SERVER["DOCUMENT_ROOT"].$fileName;

        if (file_exists($filePath)) {
            $units = array("B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB");
            $fileSize = filesize($_SERVER["DOCUMENT_ROOT"].$fileName);
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

        <div class="section">
            <table>
                <tbody>
                    <tr>
                        <td>
                            Latest snapshot
                        </td>
                        <td class="date">
                            (21 March 2013)
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>

        <table class="downloads">
            <tbody>
                <tr>
                    <td class="platform">
                        <table>
                            <tbody>
                                <tr>
                                    <td class="logo">
                                        <img src="res/pics/windows.png" width=72 height=72 alt="Windows">
                                    </td>
                                    <td>
                                        <div>
                                            Windows
                                        </div>

                                        <ul>
                                            <li><a href="http://www.opencor.ws/downloads/OpenCOR-2013-03-21-Windows.exe">Installer</a> <span class="fileSize">(<?php echo formattedFileSize("/downloads/OpenCOR-2013-03-21-Windows.exe"); ?>)</span></li>
                                            <li><a href="http://www.opencor.ws/downloads/OpenCOR-2013-03-21-Windows.zip">ZIP file</a> <span class="fileSize">(<?php echo formattedFileSize("/downloads/OpenCOR-2013-03-21-Windows.zip"); ?>)</span></li>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                    <td class="separator" />
                    <td class="platform">
                        <table>
                            <tbody>
                                <tr>
                                    <td class="logo">
                                        <img src="res/pics/linux.png" width=72 height=72 alt="Linux">
                                    </td>
                                    <td>
                                        <div>
                                            Linux
                                        </div>

                                        <ul>
                                            <li><a href="http://www.opencor.ws/downloads/OpenCOR-2013-03-21-Linux32.tar.gz">Tarball file</a> (32-bit) <span class="fileSize">(<?php echo formattedFileSize("/downloads/OpenCOR-2013-03-21-Linux32.tar.gz"); ?>)</span></li>
                                            <li><a href="http://www.opencor.ws/downloads/OpenCOR-2013-03-21-Linux64.tar.gz">Tarball file</a> (64-bit) <span class="fileSize">(<?php echo formattedFileSize("/downloads/OpenCOR-2013-03-21-Linux64.tar.gz"); ?>)</span></li>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                    <td class="separator" />
                    <td class="platform">
                        <table>
                            <tbody>
                                <tr>
                                    <td class="logo">
                                        <img src="res/pics/osx.png" width=72 height=72 alt="Linux">
                                    </td>
                                    <td>
                                        <div>
                                            Linux
                                        </div>

                                        <ul>
                                            <li><a href="http://www.opencor.ws/downloads/OpenCOR-2013-03-21-OSX.dmg">Installer</a> <span class="fileSize">(<?php echo formattedFileSize("/downloads/OpenCOR-2013-03-21-OSX.dmg"); ?>)</span></li>
                                            <li><a href="http://www.opencor.ws/downloads/OpenCOR-2013-03-21-OSX.zip">ZIP file</a> <span class="fileSize">(<?php echo formattedFileSize("/downloads/OpenCOR-2013-03-21-OSX.zip"); ?>)</span></li>
                                        </ul>
                                    </td>
                                </tr>
                            </tbody>
                        </table>
                    </td>
                </tr>
            </tbody>
        </table>

        <script type="text/javascript">
            copyright();
        </script>
    </body>
</html>
