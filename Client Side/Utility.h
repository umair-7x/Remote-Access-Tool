
string convertToString(double num) {
	ostringstream convert;
	convert << num;     
	return convert.str();
}

double roundOff(double n)
{
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
}

string convertSize(size_t size)
{                   
    static const char *SIZES[] = { "B", "KB", "MB", "GB" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1024);
        div++;  
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;
	string result = convertToString(roundOff(size_d)) + " " + SIZES[div];

	return result;
}


int file_size(const char *path) {
	struct stat results;
    
    if (stat(path, &results) == 0)
		return results.st_size;
    else
		return -1;
}

string getFileSize(string path) {
	size_t size = file_size((const char *)path.c_str());
	return convertSize(size);
}


/* -------- String Functions -------------------------------------------------- */

vector<string> split(string s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim)) {
		elems.push_back(strTrim(item));
	}
	return elems;
}

string toLowerCase(string str) {
	const int length = str.length();
	for(int i=0; i < length; ++i) {
		str[i] = tolower(str[i]);
	}
	return str;
}

string strTrim(string str) {
	size_t pos1 = str.find_first_not_of(" \t");
	size_t pos2 = str.find_last_not_of(" \t");

	str = str.substr(pos1 == string::npos ? 0 : pos1, 
		pos2 == string::npos ? 0 : pos2 - pos1 + 1);
 
	return str;
}