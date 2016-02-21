
bool UploadFile (string Path) {

	// get file name
	string FileName = Path.substr(Path.find_last_of("\\") + 1);

	string boundary = "AaB03x";
	string LastBoundary = "\r\n--" + boundary + "--";

	string Headers = "Content-Type: multipart/form-data; boundary=" + boundary;
	string FormData;
	FormData = "--" + boundary + "\r\nContent-Disposition: form-data; name=\"client\"\r\n\r\n" + ID + "\r\n";
	FormData += "--" + boundary + "\r\nContent-Disposition: form-data; name=\"file\"; filename=\"" + FileName + "\"\r\nContent-Type: application/octet-stream\r\nContent-Transfer-Encoding: binary\r\n\r\n";

	//MessageBox(NULL, FormData.c_str(), "Title", NULL);

	// open the file
	HANDLE hSrcFile = CreateFile(Path.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, NULL, NULL);
	if (hSrcFile == INVALID_HANDLE_VALUE) return false;
 
	// get file size
	DWORD dwFileSize = file_size(Path.c_str());
	
	INTERNET_BUFFERS BufferIn;

	BufferIn.dwStructSize = sizeof(INTERNET_BUFFERS); // Must be set or error will occur
	BufferIn.Next = NULL; 
	BufferIn.lpcszHeader = Headers.c_str();
	BufferIn.dwHeadersLength = Headers.length();
	//BufferIn.dwHeadersTotal = 0;
	BufferIn.lpvBuffer = NULL;                
	BufferIn.dwBufferLength = 0;
	BufferIn.dwBufferTotal = FormData.length() + dwFileSize + LastBoundary.length();
	BufferIn.dwOffsetLow = 0;
	BufferIn.dwOffsetHigh = 0;

	HINTERNET hSession = InternetOpen("MyAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(hSession != 0) {

		//INTERNET_DEFAULT_HTTP_PORT
		HINTERNET hConnect = InternetConnect(hSession, "example.com", INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);
		if(hConnect != 0)
		{
			HINTERNET hRequest = HttpOpenRequest(hConnect, "POST", "k/upload.php", NULL, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE, 1);
			
			if(hRequest != 0)
			{

			   if(HttpSendRequestEx(hRequest, &BufferIn, NULL, 0, NULL))
			   {

				   char pBuffer[1024];
				   DWORD dwTotalWritten = 0, dwBytesRead, dwBytesWritten;

				   InternetWriteFile(hRequest, FormData.c_str(), FormData.length(), &dwBytesWritten);

				   do
				   {
					   if(ReadFile(hSrcFile, &pBuffer, sizeof(pBuffer), &dwBytesRead, NULL)) {

						   if(!(InternetWriteFile(hRequest, pBuffer, dwBytesRead, &dwBytesWritten)))
						   	   return false;
						   
						   dwTotalWritten += dwBytesWritten;
					   }
					   else
					   {
						   return false;
					   }
				   } while (dwTotalWritten < dwFileSize);

				   // close handle to the file
				   CloseHandle(hSrcFile);

				   InternetWriteFile(hRequest, LastBoundary.c_str(), LastBoundary.length(), &dwBytesWritten);

				   HttpEndRequest(hRequest, NULL, 0, 0);

				   InternetCloseHandle (hRequest);
				   InternetCloseHandle (hConnect);
				   InternetCloseHandle (hSession);

				   return true;
			   }

			   InternetCloseHandle (hRequest);
			}

			InternetCloseHandle (hConnect);
	   }

	   InternetCloseHandle (hSession);
	}

	return false;
}
