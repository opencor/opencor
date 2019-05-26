<?php
    function downloads($dirName = "") {
        $realDirName = ($dirName === "")?$_SERVER["DOCUMENT_ROOT"]."/downloads":$dirName;

        foreach (array_diff(scandir($realDirName), array(".", "..")) as $fileName) {
            if (is_dir($realDirName."/".$fileName)) {
                downloads($realDirName."/".$fileName);
            } else if (strpos($fileName, "OpenCOR") !== false) {
                echo str_replace($_SERVER["DOCUMENT_ROOT"]."/downloads/", "", $realDirName."/".$fileName).",".filesize($realDirName."/".$fileName)."\n";
            }
        }
    }

    downloads();
?>
