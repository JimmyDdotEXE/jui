#ifndef JUI_H
#define JUI_H

#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Color.h"

class Window;


typedef enum {UNDEFINED, UNIX, WIN} e_os;

extern e_os sys;
extern std::string homeDir;

extern const bool BIG_END;
extern const uint R_MASK;
extern const uint G_MASK;
extern const uint B_MASK;
extern const uint A_MASK;

extern bool fullRedraw;

extern std::string fontLocation;
extern std::string iconLocation;

extern Color baseColor[];
extern Color highlightColor[];
extern Color *darkTheme[];
extern Color *lightTheme[];
extern Color *accent;

extern bool darkMode;
extern bool invertedMode;

extern bool inversionNeeded;

extern uint mouseDeadZone;
extern uint multiclickTimeout;
extern double renderScale;


bool start();
void stop();
bool ready();

void loadPalette();

bool setCursor(SDL_SystemCursor c);

bool mountWindow(Window *win);
bool unmountWindow(Window *win);

bool framePrep();
bool checkWindowThemes();
bool handleEvents();

bool frameCleanUp();
bool drawWindowControls();
bool postWindowFrames();

#endif
