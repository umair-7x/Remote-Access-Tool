<?php

	$server = "localhost";
	$user = "root";
	$pass = "";
	$database = "name";

	$connection = mysql_connect($server, $user, $pass);
	if (!mysql_select_db($database)) die(mysql_error());
	
?>