#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <dirent.h>

#ifdef unix
	#include <sys/stat.h>
#elif _WIN32
	#include <direct.h>
#endif

#include "Utility.h"


/*convert degrees to radians*/
double degreesToRadians(double d){
	return (d * M_PI) / 180.0;
}

/*convert radians to degrees*/
double radiansToDegrees(double r){
	return (r * 180.0) / M_PI;
}


/*check if a string is numeric*/
bool isNumeric(std::string s){
	if(s.length() == 0){
		return false;
	}

	for(int i=0;i<s.length();i++){
		if(!std::isdigit(s[i]) && !(s[i] == '-' && i == 0)){
			return false;
		}
	}

	return true;
}

bool isDirectory(std::string s){
	DIR *dir = NULL;
	if(dir = opendir(s.c_str())){
		closedir(dir);
		dir = NULL;
		return true;
	}else{
		return false;
	}
}


bool mkDir(std::string dir){
#ifdef unix
	return !mkdir(dir.c_str(), 0700);
#elif _WIN32
	return !_mkdir(dir.c_str());
#endif

	return false;
}


/*convert an int to a string*/
std::string numString(int num){
	std::ostringstream stream;
	stream << num;

	return stream.str();
}

/*convert a string to an int*/
int stringInt(std::string s){
	int ret = 0;

	if(isNumeric(s)){
		ret = atol(s.c_str());
	}else if(s == "#"){
		return 0;
	}else{
		s = s.substr(1);

		for(int i=0;i<s.length();i++){
			if(s[i] >= 48 && s[i] <= 57){
				ret = ret << 4 | s[i] - 48;
			}else if(s[i] >= 65 && s[i] <= 70){
				ret = ret << 4 | s[i] - 55;
			}else if(s[i] >= 97 && s[i] <= 102){
				ret = ret << 4 | s[i] - 87;
			}
		}

		ret = ret << 8 | 0x000000FF;
	}


	return ret;
}

/*shift all charcters in a string to lower-case*/
std::string stringLower(std::string s){
	for(int i=0;i<s.length();i++){
		s[i] = std::tolower(s[i]);
	}

	return s;
}


bool sortAlpha(std::vector<std::string> *v){
	for(int i=1;i<v->size();i++){
		if(stringLower(v->at(i)).compare(stringLower(v->at(i-1))) < 0){
			v->insert(v->begin()+i-1, v->at(i));
			v->erase(v->begin()+i+1);
			i = 0;
		}
	}

	return true;
}
