#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <dirent.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Color.h"
#include "General.h"


/*determine what OS the program is running on*/
#ifdef unix
	std::string homeDir(getenv("HOME"));
	e_os sys = UNIX;
#elif _WIN32
	std::string homeDir(getenv("HOMEPATH"));
	e_os sys = WIN;
#endif

bool fullRedraw = false;


Color baseColor[8];
Color highlightColor[8];
Color *darkTheme[5];
Color *lightTheme[5];
Color *accent;

bool darkMode = true;


/*initialize everything SDL needs*/
bool start(){
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "Failed to initialize SDL. Given SDL error: " << SDL_GetError() << std::endl;
		return false;
	}else if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		std::cout << "Failed to initialize SDL_image. Given SDL_image error: " << IMG_GetError() << std::endl;
		return false;
	}else if(TTF_Init() == -1){
		std::cout << "Failed to initialize SDL_ttf. Given SDL_ttf error: " << TTF_GetError() << std::endl;
		return false;
	}

	SDL_StartTextInput();

	return true;
}

/*quit everything SDL was using*/
void stop(){
	SDL_StopTextInput();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


/*load the local hex palette*/
void loadPalette(){
	std::ifstream file("");
	uint line;
	int hex;

	if(file.is_open()){

		for(int i=0;file >> std::hex >> line;i++){	
			if(i<8){
				baseColor[i] = Color(line << 8 | 0x000000ff);
			}else{
				highlightColor[i-8] = Color(line << 8 | 0x000000ff);
			}	
		}


		file.close();
	}else{
		baseColor[0] = 0x002b36ff;
		baseColor[1] = 0x073642ff;
		baseColor[2] = 0x586e75ff;
		baseColor[3] = 0x657b83ff;
		baseColor[4] = 0x839496ff;
		baseColor[5] = 0x93a1a1ff;
		baseColor[6] = 0xeee8d5ff;
		baseColor[7] = 0xfdf6e3ff;

		highlightColor[0] = 0xb58900ff;
		highlightColor[1] = 0xcb4b16ff;
		highlightColor[2] = 0xdc322fff;
		highlightColor[3] = 0xd33682ff;
		highlightColor[4] = 0x6c71c4ff;
		highlightColor[5] = 0x268bd2ff;
		highlightColor[6] = 0x2aa198ff;
		highlightColor[7] = 0x859900ff;
	}


	darkTheme[0] = &baseColor[0];
	darkTheme[1] = &baseColor[1];
	darkTheme[2] = &baseColor[4];
	darkTheme[3] = &baseColor[2];
	darkTheme[4] = &baseColor[5];

	lightTheme[0] = &baseColor[7];
	lightTheme[1] = &baseColor[6];
	lightTheme[2] = &baseColor[3];
	lightTheme[3] = &baseColor[5];
	lightTheme[4] = &baseColor[2];

	accent = &highlightColor[6];

}


/*set how the cursor looks*/
bool setCursor(SDL_SystemCursor c){
	static SDL_SystemCursor cursor = SDL_SYSTEM_CURSOR_ARROW;

	if(cursor != c){
		SDL_FreeCursor(SDL_GetCursor());

		SDL_SetCursor(SDL_CreateSystemCursor(c));

		cursor = c;
	}
}


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
	for(int i=0;i<s.length();i++){
		if(!std::isdigit(s[i]) && !(s[i] == '-' && i == 0)){
			return false;
		}
	}

	return true;
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


/*round a double*/
double round(double n){
	if(n+.5 > (int)n){
		return n+.5;
	}else{
		return n;
	}
}
