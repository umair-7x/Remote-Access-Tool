<?php

include("database.inc.php");
include("disable_magic_quotes.inc.php");

$sql = "SELECT * FROM clients";
$result = mysql_query($sql);
$total = mysql_num_rows($result);

if ($total) {

	$data = "";

	while ($row = mysql_fetch_assoc($result)) {
		
		$client = $row["client"];
		$status = $row["status"];
		
		echo $client . "|";
		echo $status . "\n";
				
		/*
		$data .= "<li>";
		if ($status == "online") $data .= "<a href=\"#\">";
		$data .= $client;
		if ($status == "online") $data .= "</a>";
		$data .= "</li>\n";
		*/
	}

	$sql = "UPDATE clients SET status = 'offline'";
	mysql_query($sql);
	
	echo $data;
}

?>