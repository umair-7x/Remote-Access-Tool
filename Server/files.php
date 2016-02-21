<html>

<head>

<style type="text/css">
	* {
		margin: 0px;
		padding: 0px;
	}
	
	a { color: #136CB2; text-decoration: none; }
	a:hover { color: #333333; text-decoration: none; }
	a.red { color: #DD2515;}
	a.red:hover { color: #333333; text-decoration: none; }
	
	body {
		font-family: arial;
		font-size: 12px;
		color: #222222;
		line-height: 16px;
	}
	
	input {
		padding: 2px;
		padding-left: 5px;
		padding-right: 5px;
	}

	#files {
		width: 100%;
		height: 150px;
		border: 1px solid #CACACA;
		overflow-y: scroll;
		margin-bottom: 10px;
		font-family: arial;
		font-size: 12px;
	}

	#files .wrapper {
		padding: 15px;
	}
	
</style>

</head>

<body>

<div id="files">
	<div class="wrapper">
	
<?php

	include("disable_magic_quotes.inc.php");

	if (isset($_FILES["file"]["name"])) {

		if ($_FILES["file"]["error"] > 0) {
			echo $_FILES["file"]["error"];
		}
		else {
			move_uploaded_file($_FILES["file"]["tmp_name"], "files/" . $_FILES["file"]["name"]);
			echo "<b>File uploaded: " .  $_FILES["file"]["name"] . "</b><br /><br />";
		}
	}
	
	if (isset($_GET["delete"])) {
	
		if (isset($_GET["file"])) $file = $_GET["file"];
		else die();
		
		@unlink("files/" . $file);
		echo "<b>$file deleted</b><br /><br />";
	}
	
	$handle = opendir("files");

	while (($file = readdir($handle)) !== false) {
		if ( ($file != ".") AND ($file != "..") ) {
			echo "<a href=\"files/$file\" target=\"_blank\">$file</a>";
			echo "&nbsp; - &nbsp;<a href=\"files.php?delete=true&file=$file\" class=\"red\">Delete</a><br />";
		}
	}

	echo "<br />";
	
	closedir($handle);

?>
	</div>
</div>

<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="POST" enctype="multipart/form-data">

	<input type="file" name="file" id="file" />
	<input type="submit" name="submit" value="Upload" />

</form>


</body>
</html>