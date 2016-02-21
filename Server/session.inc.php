<?php

session_start();

if (isset($_GET["logout"])) {
	session_unset();
	session_destroy();
}

if (isset($_POST["password"])) {
	$password = $_POST["password"];
	if ($password == "legendary") {
		$_SESSION["logged"] = true;
	}
}

if (!isset($_SESSION["logged"]))
{

?>
	<form method="POST" action="index.php">
		<input type="password" name="password" size="20" />
		<input type="submit" value="Login" />
	</form>
	
<?php 

	die();
}
?>