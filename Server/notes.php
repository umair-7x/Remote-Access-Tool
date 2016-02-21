<?php

include("database.inc.php");
include("disable_magic_quotes.inc.php");

if (isset($_POST["notes"])) {

	if (isset($_POST["client"])) $client = mysql_real_escape_string($_POST["client"]);
	else die();
	
	$notes = mysql_real_escape_string($_POST["notes"]);

	$sql = "UPDATE clients SET notes = '" . $notes . "' WHERE client = '" . $client . "'";
	mysql_query($sql);
}
else if (isset($_GET["client"])) {
	
	$client = mysql_real_escape_string($_GET["client"]);
	
	$sql = "SELECT notes FROM clients WHERE client = '" . $client . "'";
	$result = mysql_query($sql);
	$row = mysql_fetch_row($result);
	
	echo $row[0];
}

?>