string DownloadFile(const char *URL, const char *Dest)
{
	const int downloadBufferSize = 1024;

	HINTERNET hSession = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hSession == NULL) return "Error";

	/* ------------------------------------ */

	DWORD var = 5000; // milliseconds
	InternetSetOption(hSession, INTERNET_OPTION_CONNECT_TIMEOUT,
	(void *)&var, sizeof(var));
	InternetSetOption(hSession, INTERNET_OPTION_DATA_RECEIVE_TIMEOUT,
	(void *)&var, sizeof(var));
	InternetSetOption(hSession, INTERNET_OPTION_DATA_SEND_TIMEOUT,
	(void *)&var, sizeof(var));
	InternetSetOption(hSession, INTERNET_OPTION_SEND_TIMEOUT,
	(void *)&var, sizeof(var));
	InternetSetOption(hSession, INTERNET_OPTION_RECEIVE_TIMEOUT,
	(void *)&var, sizeof(var));
	var = 4; // Retries
	InternetSetOption(hSession, INTERNET_OPTION_CONNECT_RETRIES,
	(void *)&var, sizeof(var));

	/* ------------------------------------ */

	HINTERNET hFile = InternetOpenUrl(hSession, URL, NULL, 0, INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if(hFile == NULL) return "Error";

	string downloadedContents = "";

	char *downloadBuffer = new char[downloadBufferSize];
    DWORD availableSize = 0, bytesRead = 0;
	
	ofstream file;
	if (Dest != NULL) file.open(Dest, ios::binary);

	do {
		// Query how much data is available
		InternetQueryDataAvailable(hFile, &availableSize, 0, 0);

		// Read only as much as is available or downloadBufferSize
		if (availableSize > downloadBufferSize)
			availableSize = downloadBufferSize;
			
		InternetReadFile(hFile, downloadBuffer, availableSize, &bytesRead);

		// Append the buffer to the download contents
		downloadedContents.append(downloadBuffer, availableSize);
		// If second parameter is specified:
		if (Dest != NULL) file.write(downloadBuffer, bytesRead); 
	
	} while(bytesRead != 0);
	
	file.close();
	InternetCloseHandle(hFile);
	InternetCloseHandle(hSession);
	
	if (Dest != NULL)
		return "Success";
	else
		return downloadedContents;
}
