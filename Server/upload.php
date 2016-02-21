<?php

include("database.inc.php");
include("disable_magic_quotes.inc.php");

function generateUniqueId(){
	$c = substr(uniqid(rand(), false), 0, 5);
	return $c;
}

function getFileSize($filePath, $digits = 2) {
	if (is_file($filePath)) {
		$fileSize = filesize($filePath);
		$sizes = array("TB","GB","MB","KB","B");
		$total = count($sizes);
		while ($total-- && $fileSize > 1024) {
			   $fileSize /= 1024;
		}
		return round($fileSize, $digits)." ".$sizes[$total];
	}
	return false;
}

if (isset($_FILES["file"]["name"])) {

	if ($_FILES["file"]["error"] > 0) {
		echo $_FILES["file"]["error"];
	}
	else {
		$client = $_POST["client"];
		$source = $_FILES["file"]["tmp_name"];
		$filename = $_FILES["file"]["name"];
		$size = getFileSize($source);
		
		if (substr($filename, 0, 1) == "!") {
			$target = "uploads/" . substr($filename, 1);
			move_uploaded_file($source, $target);
		}
		else {
			$new_filename = generateUniqueId() . "-" . $filename;
			$target = "uploads/" . $new_filename;
			move_uploaded_file($source, $target);
		
			$client = mysql_real_escape_string($client);
			$filename = mysql_real_escape_string($filename);
			$new_filename = mysql_real_escape_string($new_filename);
			
			$sql = "INSERT INTO uploads (name, path, size, client) VALUES ('" . $filename . "', '" . $new_filename . "', '" . $size . "', '" . $client . "')";
			mysql_query ($sql);
		}
				
		echo "Stored as: " .  $target;
	}
}
else if (isset($_GET["client"])) {

	$client = mysql_real_escape_string($_GET["client"]);
	if (empty($client)) die();
	
	$sql = "SELECT * FROM uploads WHERE client = '" . $client . "'";
	$result = mysql_query($sql);
	$total = mysql_num_rows($result);

	if ($total) {
		while ($row = mysql_fetch_assoc($result)) {
			echo '<div>';
			echo '<a href="uploads/' . $row["path"] . '" target="_blank">' . $row["name"] . '</a> <span>' . $row["size"] . '</span>';
			echo ' - <a href="upload.php?delete=true&id=' . $row["id"] . '" id="deleteUpload" class="red">Delete</a><br />';
			echo '</div>';
		}
	}
	else {
		echo "No files uploaded.";
	}
}
else if (isset($_GET["delete"])) {
	
	if (isset($_GET["id"])) $id = $_GET["id"];
	else die();
	
	$sql = "SELECT * FROM uploads WHERE id = " . $id;
	$result = mysql_query($sql);
	$total = mysql_num_rows($result);

	if ($total) {
		$row = mysql_fetch_assoc($result);
		$path = $row["path"];
		@unlink("uploads/" . $path);
		$sql = "DELETE FROM uploads WHERE id = " . $id;
		mysql_query($sql);
		
		echo $row["name"] . " deleted.";
	}
}
else {
?>

<html>
<body>

	<form action="upload.php" method="post" enctype="multipart/form-data">

		<label for="client">Client:</label>
		<input type="text" name="client" />
		<br />
		<label for="file">Filename:</label>
		<input type="file" name="file" />
		<br />
		<input type="submit" name="submit" value="Submit" />

	</form>

</body>
</html>


<?php
}
?>