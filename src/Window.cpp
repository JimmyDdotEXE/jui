#include <iostream>
#include <SDL2/SDL_image.h>
#include "JUI.h"
#include "Window.h"


/*
	Window constructor

	uint w: window width
	uint h: window height
	std::string title: window title
	bool resize: is the window resizable
 */
Window::Window(uint w, uint h, std::string title, bool resize, Window *parent){
	window = NULL;
	renderer = NULL;

	parentWindow = parent;
	modalWindow = NULL;

	id = -1;

	this->title = title;
	resizable = resize;

	width = w;
	height = h;

	windowDark = darkMode;
	windowAccent = *accent;
	windowInverted = invertedMode;

	if(darkMode){
		clear(*darkTheme[0]);
	}else{
		clear(*lightTheme[0]);
	}

	activeControl = NULL;
}


/*get the width of the window*/
int Window::getWidth(){
	return width;
}

/*get the height of the window*/
int Window::getHeight(){
	return height;
}

/*get the title of the window*/
std::string Window::getTitle(){
	return title;
}

int Window::getID(){
	return id;
}

SDL_Window *Window::getSDL_Window(){
	return window;
}

Window *Window::getParentWindow(){
	return parentWindow;
}

Window *Window::getModalWindow(){
	return modalWindow;
}


/*set the width of the window*/
void Window::setWidth(uint w){

	if(w != width){

		for(int i=0;i<controls.size();i++){
			Control *control = controls.at(i);

			if(control->getLeftLock() && control->getRightLock()){
				bool noResize = true;

				if(control->getLockResize()){
					noResize = !control->setWidth(control->getWidth() + ((int)w - (int)width));
				}

				if(noResize){
					double ratio = (control->getX() + (double)(control->getWidth()/2)) / (double)width;
					control->setX((double)w*ratio - (double)(control->getWidth()/2));
				}
			}else if(control->getRightLock()){
				control->setX(control->getX() + ((int)w - (int)width));
			}
		}

		width = w;
	}
}

/*set the height of the window*/
void Window::setHeight(uint h){

	if(h != height){

		for(int i=0;i<controls.size();i++){
			Control *control = controls.at(i);

			if(control->getTopLock() && control->getBottomLock()){
				bool noResize = true;

				if(control->getLockResize()){
					noResize = !control->setHeight(control->getHeight() + ((int)h - (int)height));
				}

				if(noResize){
					double ratio = (control->getY() + (double)(control->getHeight()/2)) / (double)height;
					control->setY((double)h*ratio - (double)(control->getHeight()/2));
				}
			}else if(control->getBottomLock()){
				control->setY(control->getY() + ((int)h - (int)height));
			}
		}

		height = h;
	}
}

/*set the title of the window*/
bool Window::setTitle(std::string s){
	title = s;

	if(isReady()){
		SDL_SetWindowTitle(window, s.c_str());
	}

	return title == s;
}

bool Window::setModalWindow(Window *modal){
	if(modal == NULL || modal->getParentWindow() == this){
		modalWindow = modal;
	}

	return modalWindow == modal;
}


/*tells if the window is ready to be used*/
bool Window::isReady(){
	return window != NULL && renderer != NULL;
}

/*closes the window*/
void Window::close(){
	unmountWindow(this);
}

//TODO: find a way to prevent bypassing system mount call
bool Window::mount(){
	if(!isReady()){
		if(resizable){
			window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_RESIZABLE);
		}else{
			window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		}


		if(window){
			id = SDL_GetWindowID(window);

			if(parentWindow != NULL){
				SDL_SetWindowModalFor(window, parentWindow->getSDL_Window());
			}

			SDL_Surface *icon = IMG_Load(iconLocation.c_str());
			SDL_SetWindowIcon(window, icon);
			SDL_FreeSurface(icon);


			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

			if(renderer){
				SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
				clear();

				if(parentWindow != NULL){
					parentWindow->setModalWindow(this);
				}
			}else{
				std::cout << "Failed to create renderer. Given SDL error: " << SDL_GetError() << std::endl;

				return false;
			}
		}else{
			std::cout << "Failed to create window. Given SDL error: " << SDL_GetError() << std::endl;
			return false;
		}
	}else{
		std::cout << "Failed to mount window. Already mounted." << std::endl;
		return false;
	}

	return true;
}

//TODO: find a way to prevent bypassing system unmount call
bool Window::unmount(){
	if(parentWindow != NULL){
		parentWindow->setModalWindow(NULL);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	window = NULL;
	renderer = NULL;

	id = -1;

	return true;
}


//this function exists as rough fix for renderer issues on secondary window close
bool Window::refreshRenderer(){
	if(isReady()){
		SDL_DestroyRenderer(renderer);
		renderer = NULL;

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		if(renderer){
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			clear();

			if(fullRedraw == false){
				for(int i=0;i<controls.size();i++){
					controls.at(i)->updateTheme();
				}
			}

			return true;
		}else{
			std::cout << "Failed to create renderer. Given SDL error: " << SDL_GetError() << std::endl;
		}
	}

	return false;
}


bool Window::handleEvent(Event *event){

	if(event->getType() == e_WINDOW){
		switch(event->getWindowEvent()){
			case w_SHOWN:
				hidden = false;
				break;
			case w_HIDDEN:
				hidden = true;
				break;
			case w_EXPOSED:
				//TODO: figure out what to do with an exposed event
				break;
			case w_MOVED:
				//TODO: figure out what to do with a move event
				break;
			case w_RESIZED:
				setWidth(event->getWidth());
				setHeight(event->getHeight());

				//redraw everything on resize on windows
				//could probably be a window redraw instead of program wide
				if(sys == WIN){
					fullRedraw = true;
				}

				break;
			case w_SIZECHANGED:
				setWidth(event->getWidth());
				setHeight(event->getHeight());
				break;
			case w_MINIMIZED:
				minimized = true;
				maximized = false;
				break;
			case w_MAXIMIZED:
				minimized = false;
				maximized = true;
				break;
			case w_RESTORED:
				minimized = false;
				maximized = false;
				break;
			case w_ENTER:
				mouseFocus = true;
				break;
			case w_LEAVE:
				mouseFocus = false;
				break;
			case w_FOCUSGAINED:
				keyboardFocus = true;

				//redraw everything on focus gain on windows
				//could probably be a window redraw instead of program wide
				if(sys == WIN){
					fullRedraw = true;
				}

				break;
			case w_FOCUSLOST:
				keyboardFocus = false;
				break;
			case w_CLOSE:
				//TODO: figure out what to do on close
				break;
		}
	}

	handleControls(event);
	return true;
}

/*see which controls have a use for the given event*/
void Window::handleControls(Event *event){
	if(activeControl != NULL && activeControl->getVisible()){
		activeControl->handleEvent(event);

		if(event->getControl() == activeControl){
			if(!activeControl->getActive()){
				activeControl = NULL;
			}
		}
	}

	for(int i=controls.size()-1;i>=0;i--){

		if(controls.at(i) != activeControl && controls.at(i)->getVisible()){
			controls.at(i)->handleEvent(event);

			if(event->getControl() == controls.at(i)){
				if(activeControl != NULL){
					activeControl->setActive(false);
					activeControl = NULL;
				}

				if(controls.at(i)->getActive()){
					activeControl = controls.at(i);
				}

			}
		}

	}

}


/*clear the window*/
void Window::clear(Color color){
	clearColor = color;

	if(!isReady()){
		return;
	}else{
		clear();
	}
}

void Window::clear(){
	SDL_SetRenderDrawColor(renderer, clearColor.getRed(), clearColor.getGreen(), clearColor.getBlue(), clearColor.getAlpha());
	SDL_RenderClear(renderer);
}

/*post the prepared frame to the window*/
bool Window::postFrame(){
	if(!isReady()){
		return false;
	}

	SDL_RenderPresent(renderer);
	return true;
}


/*draw the given object*/
void Window::draw(DrawObject *object){
	if(!isReady()){
		return;
	}

	object->draw(renderer);
}


/*add the given control*/
void Window::addControl(Control *control){
	control->setWindow(this);
	controls.push_back(control);
}

/*remove the given control*/
void Window::removeControl(Control *control){
	for(int i=0;i < controls.size();i++){
		if(controls.at(i) == control){
			controls.erase(controls.begin() + i);
			return;
		}
	}
}

/*draw all controls*/
bool Window::drawControls(){
	bool ret = true;

	if(!isReady()){
		return false;
	}

	if(isReady()){
		for(int i=0;i<controls.size();i++){
			if(controls.at(i)->getVisible()){
				ret = ret && controls.at(i)->draw(renderer);
			}
		}
	}

	return ret;
}


bool Window::checkWindowTheme(){
	bool ret = true;

	if(darkMode && clearColor.getUint() == lightTheme[0]->getUint()){
		clear(*darkTheme[0]);
	}else if(!darkMode && clearColor.getUint() == darkTheme[0]->getUint()){
		clear(*lightTheme[0]);
	}

	clear();

	//if the window's theme doesn't match the program theme
	if(windowDark != darkMode || windowAccent.getUint() != accent->getUint()){
		for(int i=0;i < controls.size();i++){
			ret = ret && controls.at(i)->updateTheme();
		}

		windowDark = darkMode;
		windowAccent = *accent;
	}

	if(windowInverted != invertedMode){
		fullRedraw = true;
		inversionNeeded = true;

		windowInverted = invertedMode;
	}

	return ret;
}

/*update the theme of the controls in the window*/
bool Window::updateTheme(){
	bool ret = true;

	for(int i=0;i<controls.size();i++){
		ret = ret && controls.at(i)->updateTheme();
	}

	return ret;
}
