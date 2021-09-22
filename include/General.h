#ifndef GENERAL_H
#define GENERAL_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Color;

typedef unsigned int uint;
typedef unsigned char uchar;
typedef enum {UNDEFINED, UNIX, WIN} e_os;

extern e_os sys;
extern std::string homeDir;

extern bool fullRedraw;

extern std::string fontLocation;
extern std::string iconLocation;

extern Color baseColor[];
extern Color highlightColor[];
extern Color *darkTheme[];
extern Color *lightTheme[];
extern Color *accent;

extern bool darkMode;


bool start();
void stop();

void loadPalette();

bool setCursor(SDL_SystemCursor c);

double degreesToRadians(double d);
double radiansToDegrees(double r);

bool isNumeric(std::string s);

std::string numString(int num);
int stringInt(std::string s);
std::string stringLower(std::string s);

double round(double n);

#endif
