<?php

$site = $_GET["site"];
$logs = $_GET["logs"];
$ID = $_GET["ID"];

$file = fopen("logs.txt", "a+");
fwrite($file, $ID . " - " . $site . " - " . $logs . "\n");
fclose($file);

?>
