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
#include "Event.h"
#include "Window.h"
#include "JUI.h"


/*determine what OS the program is running on*/
#ifdef unix
	std::string homeDir(getenv("HOME"));
	e_os sys = UNIX;
#elif _WIN32
	std::string homeDir(getenv("HOMEPATH"));
	e_os sys = WIN;
#endif

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	const bool BIG_END = true;

	const uint R_MASK = 0x000000ff;
	const uint G_MASK = 0x0000ff00;
	const uint B_MASK = 0x00ff0000;
	const uint A_MASK = 0xff000000;
#else
	const bool BIG_END = false;

	const uint R_MASK = 0xff000000;
	const uint G_MASK = 0x00ff0000;
	const uint B_MASK = 0x0000ff00;
	const uint A_MASK = 0x000000ff;
#endif

bool fullRedraw = false;
uint lastFullRedraw = SDL_GetTicks();

Color baseColor[8];
Color highlightColor[8];
Color *darkTheme[5];
Color *lightTheme[5];
Color *accent;

bool darkMode = true;
bool invertedMode = false;

bool inversionNeeded = false;

uint multiclickTimeout = 500;
double renderScale = 1;

bool checkDeadZone;

std::unordered_map<int, Event *> eventMap;
std::unordered_map<int, Window *> windowMap;


Event *handleMouseButtonDown(Event *event, SDL_Event e);
Event *handleMouseButtonUp(Event *event, SDL_Event e);
Event *handleMouseMotion(Event *event, SDL_Event e);
Event *handleMouseWheel(Event *event, SDL_Event e);
Event *handleTextInput(Event *event, SDL_Event e);
Event *handleKeyDown(Event *event, SDL_Event e);
Event *handleKeyUp(Event *event, SDL_Event e);


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

	SDL_DisplayMode current;
	uint maxPixels = 1920 * 1080;

	for(int i=0;i<SDL_GetNumVideoDisplays();i++){
		if(SDL_GetCurrentDisplayMode(i, &current) == 0){
			if(maxPixels < current.w * current.h){
				maxPixels = current.w * current.h;
			}
		}
	}

	renderScale = (double)maxPixels / (double)(1920*1080);
	return true;
}

/*quit everything SDL was using*/
void stop(){
	SDL_StopTextInput();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool ready(){
	return !windowMap.empty();
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

	return cursor == c;
}


bool mountWindow(Window *win){
	if(win == NULL){
		return false;
	}else if(windowMap.find(win->getID()) != windowMap.end()){
		return false;
	}

	if(win->mount()){
		windowMap[win->getID()] = win;
		eventMap[win->getID()] = NULL;

		return true;
	}

	return false;
}

bool unmountWindow(Window *win){
	if(windowMap.find(win->getID()) != windowMap.end()){
		windowMap.erase(win->getID());
		eventMap.erase(win->getID());

		win->unmount();

		//rebuild renderers for remaining windows and redraw everything
		//this has to happen to handle blendmode issues after closing secondary windows
		fullRedraw = true;
		std::unordered_map<int, Window *>::iterator it;

		//only rebuild renderers of other windows on unix systems
		if(sys == UNIX){
			for(it = windowMap.begin();it != windowMap.end();it++){
				it->second->refreshRenderer();
			}
		}

		return true;
	}

	return false;
}


bool framePrep(){
	return checkWindowThemes() && handleEvents();
}

bool checkWindowThemes(){
	bool ret = true;
	std::unordered_map<int, Window *>::iterator it;

	for(it = windowMap.begin();it != windowMap.end();it++){
		ret = ret && it->second->checkWindowTheme();
	}

	return ret;
}

bool handleEvents(){
	static int windowID;
	bool eventNeeded = true;

	SDL_Event e;
	while(SDL_PollEvent(&e)){
		if(eventMap.find(windowID) != eventMap.end()){
			if(eventMap[windowID] != NULL){
				Event *event = eventMap[windowID];

				if(event->getType() == e_KEYPRESS && event->getKeyState() == s_UP ||
						event->getType() == e_SCROLL ||
						event->getType() == e_TEXTENTRY ||
						event->getType() == e_WINDOW){
					Event *tail = event->getTail();

					delete event;
					event = tail;
					eventMap[windowID] = event;
				}
			}
		}

		std::unordered_map<int, Window *>::iterator it;

		switch(e.type){
			case SDL_WINDOWEVENT:
				windowID = e.window.windowID;

				it = windowMap.find(windowID);
				while(it != windowMap.end() && it->second->getModalWindow() != NULL){
					windowID = it->second->getModalWindow()->getID();
					it = windowMap.find(windowID);

					SDL_RaiseWindow(it->second->getSDL_Window());
					SDL_SetWindowInputFocus(it->second->getSDL_Window());
					eventNeeded = false;
				}

				if(windowID != e.window.windowID || eventMap.find(windowID) == eventMap.end()){
					continue;
				}

				switch(e.window.event){
					case SDL_WINDOWEVENT_SHOWN:
						eventMap[windowID] = new Event(w_SHOWN, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_HIDDEN:
						eventMap[windowID] = new Event(w_HIDDEN, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_EXPOSED:
						eventMap[windowID] = new Event(w_EXPOSED, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_MOVED:
						eventMap[windowID] = new Event(w_MOVED, e.window.data1, e.window.data2, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_RESIZED:
						eventMap[windowID] = new Event(w_RESIZED, e.window.data1, e.window.data2, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						eventMap[windowID] = new Event(w_SIZECHANGED, e.window.data1, e.window.data2, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
						eventMap[windowID] = new Event(w_MINIMIZED, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						eventMap[windowID] = new Event(w_MAXIMIZED, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_RESTORED:
						eventMap[windowID] = new Event(w_RESTORED, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_ENTER:
						eventMap[windowID] = new Event(w_ENTER, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_LEAVE:
						eventMap[windowID] = new Event(w_LEAVE, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						eventMap[windowID] = new Event(w_FOCUSGAINED, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						eventMap[windowID] = new Event(w_FOCUSLOST, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					case SDL_WINDOWEVENT_CLOSE:
						eventMap[windowID] = new Event(w_CLOSE, e.window.timestamp, windowID, eventMap[windowID]);
						break;
					default:
						continue;
						break;
				}

				break;
			case SDL_MOUSEBUTTONDOWN:
				eventMap[windowID] = handleMouseButtonDown(eventMap[windowID], e);
				break;
			case SDL_MOUSEBUTTONUP:
				eventMap[windowID] = handleMouseButtonUp(eventMap[windowID], e);
				break;
			case SDL_MOUSEMOTION:
				eventMap[windowID] = handleMouseMotion(eventMap[windowID], e);
				break;
			case SDL_MOUSEWHEEL:
				eventMap[windowID] = handleMouseWheel(eventMap[windowID], e);
				break;
			case SDL_TEXTINPUT:
				eventMap[windowID] = handleTextInput(eventMap[windowID], e);
				break;
			case SDL_KEYDOWN:
				eventMap[windowID] = handleKeyDown(eventMap[windowID], e);
				break;
			case SDL_KEYUP:
				eventMap[windowID] = handleKeyUp(eventMap[windowID], e);
				break;
			default:
				continue;
				break;
		}

		if(windowMap.find(windowID) != windowMap.end()){
			if(eventMap[windowID] != NULL){
				eventNeeded = false;
				windowMap[windowID]->handleEvent(eventMap[windowID]);
			}
		}
	}


	if(eventMap.find(windowID) != eventMap.end()){
		//if an event still needs to be generated
		if(eventNeeded){
			Event *event = eventMap[windowID];

			Event *temp = event;
			Event *prev = NULL;
			while(temp){
				if(event->getType() == e_HOVER){
					if(prev != NULL){
						prev->setTail(temp->getTail());
					}else{
						event = temp->getTail();
					}

					event = new Event(temp->getX(), temp->getY(), SDL_GetTicks(), windowID, event);
					delete temp;
					break;
				}else if(event->getType() == e_HOLD){	
					if(prev != NULL){
						prev->setTail(temp->getTail());
					}else{
						event = temp->getTail();
					}

					event = new Event(temp->getX(), temp->getY(), temp->getButton(), SDL_GetTicks(), windowID, event);
					delete temp;
					break;
				}else if(temp->getType() == e_MOVE){
					event = new Event(temp->getX(), temp->getY(), SDL_GetTicks(), windowID, event);
					break;
				}else if(temp->getType() == e_DRAG){
					event = new Event(temp->getX(), temp->getY(), temp->getButton(), SDL_GetTicks(), windowID, event);
					break;
				}else if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_DOWN){
					event = new Event(temp->getX(), temp->getY(), temp->getButton(), SDL_GetTicks(), windowID, event);
					break;
				}else if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_UP){
					if(prev != NULL){
						prev->setTail(temp->getTail());
					}else{
						event = temp->getTail();
					}

					event = new Event(temp->getX(), temp->getY(), SDL_GetTicks(), windowID, event);
					delete temp;
					break;
				}	

				prev = temp;
				temp = temp->getTail();
			}


			if(temp == NULL){
				int x;
				int y;

				uint state = SDL_GetMouseState(&x, &y);

				event = new Event(x, y, SDL_GetTicks(), windowID, event);
			}

			eventMap[windowID] = event;
			windowMap[windowID]->handleEvent(eventMap[windowID]);
		}
	}

	return true;
}


bool frameCleanUp(){
	return drawWindowControls() && postWindowFrames();
}

bool drawWindowControls(){
	bool ret = true;
	std::unordered_map<int, Window *>::iterator it;

	for(it = windowMap.begin();it != windowMap.end();it++){
		ret = ret && it->second->drawControls();
	}

	return ret;
}

bool postWindowFrames(){
	bool ret = true;
	std::unordered_map<int, Window *>::iterator it;

	for(it = windowMap.begin();it != windowMap.end();it++){
		ret = ret && it->second->postFrame();
	}

	if(fullRedraw){
		fullRedraw = false;
		lastFullRedraw = SDL_GetTicks();
	}

	inversionNeeded = false;
	return ret;
}




Event *handleMouseButtonDown(Event *event, SDL_Event e){
	checkDeadZone = true;

	Event *hold = NULL;
	Event *holdPrev = NULL;

	Event *temp = event;
	Event *prev = NULL;
	while(temp){
		if(temp->getType() == e_MOVE || temp->getType() == e_HOVER){
			if(prev != NULL){
				prev->setTail(temp->getTail());
				delete temp;
			}else{
				event = temp->getTail();
				delete temp;
			}

			if(e.button.button == SDL_BUTTON_LEFT){
				event = new Event(e.button.x, e.button.y, b_LEFT, s_DOWN, e.button.timestamp, e.button.windowID, event);
			}else if(e.button.button == SDL_BUTTON_RIGHT){
				event = new Event(e.button.x, e.button.y, b_RIGHT, s_DOWN, e.button.timestamp, e.button.windowID, event);
			}else if(e.button.button == SDL_BUTTON_MIDDLE){
				event = new Event(e.button.x, e.button.y, b_MIDDLE, s_DOWN, e.button.timestamp, e.button.windowID, event);
			}

			break;
		}else if(temp->getType() == e_DRAG){
			checkDeadZone = false;
			break;
		}else if(temp->getType() == e_HOLD){
			hold = temp;
			holdPrev = prev;
		}else if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_DOWN){
			if(temp->getControl() == NULL){
				if(prev == hold && prev != NULL){
					if(holdPrev != NULL){
						holdPrev->setTail(temp->getTail());
						delete hold;
						delete temp;
					}else{
						event = temp->getTail();
						delete hold;
						delete temp;
					}
				}else{
					if(hold != NULL){
						if(holdPrev != NULL){
							holdPrev->setTail(hold->getTail());
							delete hold;
						}else{
							event = hold->getTail();
							delete hold;
						}
					}

					if(prev != NULL){
						prev->setTail(temp->getTail());
						delete temp;
					}else{
						event = temp->getTail();
						delete temp;
					}
				}

				if(e.button.button == SDL_BUTTON_LEFT){
					event = new Event(e.button.x, e.button.y, b_LEFT, s_DOWN, e.button.timestamp, e.button.windowID, event);
				}else if(e.button.button == SDL_BUTTON_RIGHT){
					event = new Event(e.button.x, e.button.y, b_RIGHT, s_DOWN, e.button.timestamp, e.button.windowID, event);
				}else if(e.button.button == SDL_BUTTON_MIDDLE){
					event = new Event(e.button.x, e.button.y, b_MIDDLE, s_DOWN, e.button.timestamp, e.button.windowID, event);
				}

				break;
			}else{
				checkDeadZone = false;
				break;
			}

		}else if(temp->getType() == e_CLICK){
			if(e.button.timestamp - temp->getTimeStamp() > multiclickTimeout ||
					temp->getButton() == b_LEFT && e.button.button != SDL_BUTTON_LEFT ||
					temp->getButton() == b_RIGHT && e.button.button != SDL_BUTTON_RIGHT ||
					temp->getButton() == b_MIDDLE && e.button.button != SDL_BUTTON_MIDDLE){
				if(prev != NULL){
					prev->setTail(temp->getTail());
					delete temp;
				}else{
					event = temp->getTail();
					delete temp;
				}

				if(e.button.button == SDL_BUTTON_LEFT){
					event = new Event(e.button.x, e.button.y, b_LEFT, s_DOWN, e.button.timestamp, e.button.windowID, event);
				}else if(e.button.button == SDL_BUTTON_RIGHT){
					event = new Event(e.button.x, e.button.y, b_RIGHT, s_DOWN, e.button.timestamp, e.button.windowID, event);
				}else if(e.button.button == SDL_BUTTON_MIDDLE){
					event = new Event(e.button.x, e.button.y, b_MIDDLE, s_DOWN, e.button.timestamp, e.button.windowID, event);
				}

				break;
			}
		}

		prev = temp;
		temp = temp->getTail();
	}

	return event;
}

Event *handleMouseButtonUp(Event *event, SDL_Event e){
	Event *press = NULL;
	Event *click = NULL;

	Event *temp = event;
	Event *prev = NULL;
	while(temp){
		if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_DOWN){
			if(temp->getButton() == b_LEFT && e.button.button == SDL_BUTTON_LEFT ||
					temp->getButton() == b_RIGHT && e.button.button == SDL_BUTTON_RIGHT ||
					temp->getButton() == b_MIDDLE && e.button.button == SDL_BUTTON_MIDDLE){
				press = temp;

				if(prev != NULL){
					prev->setTail(temp->getTail());
				}else{
					event = temp->getTail();
				}

				temp = temp->getTail();
				continue;
			}
		}else if(temp->getType() == e_CLICK){
			if(e.button.timestamp - temp->getTimeStamp() > multiclickTimeout){
				if(prev != NULL){
					prev->setTail(temp->getTail());
				}else{
					event = temp->getTail();
				}

				Event *holder = temp->getTail();
				delete temp;
				temp = holder;
				break;
			}

			if(press != NULL){
				if(temp->getButton() == b_LEFT && e.button.button == SDL_BUTTON_LEFT ||
						temp->getButton() == b_RIGHT && e.button.button == SDL_BUTTON_RIGHT ||
						temp->getButton() == b_MIDDLE && e.button.button == SDL_BUTTON_MIDDLE){
					click = temp;

					if(prev != NULL){
						prev->setTail(temp->getTail());
					}else{
						event = temp->getTail();
					}

					break;
				}
			}else{
				break;
			}
		}else if(temp->getType() == e_HOLD || temp->getType() == e_DRAG){
			if(prev != NULL){
				prev->setTail(temp->getTail());
			}else{
				event = temp->getTail();
			}

			Event *holder = temp->getTail();
			delete temp;
			temp = holder;
			continue;
		}

		prev = temp;
		temp = temp->getTail();
	}

	if(click){
		if(click->getClick() == c_SINGLE){
			event = new Event(e.button.x, e.button.y, click->getButton(), c_DOUBLE, e.button.timestamp, e.button.windowID, event);
		}else if(click->getClick() == c_DOUBLE){
			event = new Event(e.button.x, e.button.y, click->getButton(), c_TRIPLE, e.button.timestamp, e.button.windowID, event);
		}else if(click->getClick() == c_TRIPLE){
			event = new Event(e.button.x, e.button.y, click->getButton(), c_SINGLE, e.button.timestamp, e.button.windowID, event);
		}

		delete click;
		delete press;
	}else if(press){
		event = new Event(e.button.x, e.button.y, press->getButton(), c_SINGLE, e.button.timestamp, e.button.windowID, event);
		delete press;
	}else{
		if(e.button.button == SDL_BUTTON_LEFT){
			event = new Event(e.button.x, e.button.y, b_LEFT, s_UP, e.button.timestamp, e.button.windowID, event);
		}else if(e.button.button == SDL_BUTTON_RIGHT){
			event = new Event(e.button.x, e.button.y, b_RIGHT, s_UP, e.button.timestamp, e.button.windowID, event);
		}else if(e.button.button == SDL_BUTTON_MIDDLE){
			event = new Event(e.button.x, e.button.y, b_MIDDLE, s_UP, e.button.timestamp, e.button.windowID, event);
		}
	}

	return event;
}

Event *handleMouseMotion(Event *event, SDL_Event e){
	Event *temp = event;
	Event *prev = NULL;
	while(temp){
		if(temp->getType() == e_MOVE || temp->getType() == e_HOVER){
			if(prev != NULL){
				prev->setTail(temp->getTail());
			}else{
				event = temp->getTail();
			}

			event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), e.motion.timestamp, e.motion.windowID, event);
			delete temp;
			break;
		}else if(temp->getType() == e_DRAG){
			if(prev != NULL){
				prev->setTail(temp->getTail());
			}else{
				event = temp->getTail();
			}

			event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), temp->getButton(), e.motion.timestamp, e.motion.windowID, event);
			delete temp;
			break;
		}else if(temp->getType() == e_HOLD){
			double moveDistance = std::sqrt(std::pow((double)e.motion.x - (double)temp->getX(), 2) +
																			std::pow((double)e.motion.y - (double)temp->getY(), 2));

			if(moveDistance < mouseDeadZone && checkDeadZone/* && !temp->getControl()->isDeadZoneIgnored()*/){
				break;
			}else{
				checkDeadZone = false;
			}

			if(prev != NULL){
				prev->setTail(temp->getTail());
			}else{
				event = temp->getTail();
			}

			Event *holder = temp->getTail();
			delete temp;
			temp = holder;
			continue;
		}else if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_DOWN){
			if(prev != NULL){
				prev->setTail(temp->getTail());
			}else{
				event = temp->getTail();
			}

			double moveDistance = std::sqrt(std::pow((double)e.motion.x - (double)temp->getX(), 2) +
																			std::pow((double)e.motion.y - (double)temp->getY(), 2));

			if(temp->getControl() == NULL){
				event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), e.motion.timestamp, e.motion.windowID, event);
			}else/* if(moveDistance >= mouseDeadZone)*/{
				event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), temp->getButton(), e.motion.timestamp, e.motion.windowID, event);
			/*}else{
				break;*/
			}

			delete temp;
			break;
		}

		prev = temp;
		temp = temp->getTail();
	}

	return event;
}

Event *handleMouseWheel(Event *event, SDL_Event e){
	int x;
	int y;

	uint state = SDL_GetMouseState(&x, &y);

	EVENT_Modifier mod = m_NONE;
	if(SDL_GetModState() & KMOD_SHIFT){
		mod = m_SHIFT;
	}

	if(e.wheel.y > 0){
		event = new Event(x, y, w_UP, mod, e.wheel.timestamp, e.wheel.windowID, event);
	}else if(e.wheel.y < 0){
		event = new Event(x, y, w_DOWN, mod, e.wheel.timestamp, e.wheel.windowID, event);
	}else if(e.wheel.x > 0){
		event = new Event(x, y, w_LEFT, mod, e.wheel.timestamp, e.wheel.windowID, event);
	}else if(e.wheel.x < 0){
		event = new Event(x, y, w_RIGHT, mod, e.wheel.timestamp, e.wheel.windowID, event);
	}

	return event;
}

Event *handleTextInput(Event *event, SDL_Event e){
	event = new Event(e.text.text, e.text.timestamp, e.text.windowID, event);
	return event;
}

Event *handleKeyDown(Event *event, SDL_Event e){
	if(SDL_GetModState() & KMOD_SHIFT){
		event = new Event(e.key.keysym.sym, s_DOWN, m_SHIFT, e.key.timestamp, e.key.windowID, event);
	}else if(SDL_GetModState() & KMOD_CTRL){
		event = new Event(e.key.keysym.sym, s_DOWN, m_CONTROL, e.key.timestamp, e.key.windowID, event);
	}else if(SDL_GetModState() & KMOD_ALT){
		event = new Event(e.key.keysym.sym, s_DOWN, m_ALT, e.key.timestamp, e.key.windowID, event);
	}else{
		event = new Event(e.key.keysym.sym, s_DOWN, m_NONE, e.key.timestamp, e.key.windowID, event);
	}

	return event;
}

Event *handleKeyUp(Event *event, SDL_Event e){
	Event *temp = event;
	Event *prev = NULL;
	while(temp){
		if(temp->getType() == e_KEYPRESS && temp->getKeyState() == s_DOWN && temp->getKey() == e.key.keysym.sym){
			if(prev != NULL){
				prev->setTail(temp->getTail());
				delete temp;
				break;
			}else{
				event = temp->getTail();
				delete temp;
				break;
			}
		}

		prev = temp;
		temp = temp->getTail();
	}

	if(SDL_GetModState() & KMOD_SHIFT){
		event = new Event(e.key.keysym.sym, s_UP, m_SHIFT, e.key.timestamp, e.key.windowID, event);
	}else if(SDL_GetModState() & KMOD_CTRL){
		event = new Event(e.key.keysym.sym, s_UP, m_CONTROL, e.key.timestamp, e.key.windowID, event);
	}else if(SDL_GetModState() & KMOD_ALT){
		event = new Event(e.key.keysym.sym, s_UP, m_ALT, e.key.timestamp, e.key.windowID, event);
	}else{
		event = new Event(e.key.keysym.sym, s_UP, m_NONE, e.key.timestamp, e.key.windowID, event);
	}

	return event;
}
