#include <iostream>
#include <SDL2/SDL_image.h>
#include "Window.h"


/*
	Window constructor

	uint w: window width
	uint h: window height
	std::string title: window title
	bool resize: is the window resizable
 */
Window::Window(uint w, uint h, std::string title, bool resize){
	window = NULL;
	renderer = NULL;


	if(resize){
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_RESIZABLE);
	}else{
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
	}


	if(window){

		SDL_Surface *icon = IMG_Load(iconLocation.c_str());
		SDL_SetWindowIcon(window, icon);
		SDL_FreeSurface(icon);


		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		if(renderer){
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		}else{
			std::cout << "Failed to create renderer. Given SDL error: " << SDL_GetError() << std::endl;
		}
	}else{
		std::cout << "Failed to create window. Given SDL error: " << SDL_GetError() << std::endl;
	}


	width = w;
	height = h;

	windowDark = darkMode;
	windowAccent = *accent;

	event = NULL;
	activeControl = NULL;
}


/*get the width of the window*/
int Window::getWidth(){
	int w;

	SDL_GetWindowSize(window, &w, NULL);

	return w;
}

/*get the height of the window*/
int Window::getHeight(){
	int h;

	SDL_GetWindowSize(window, NULL, &h);

	return h;
}

/*get the title of the window*/
std::string Window::getTitle(){
	return SDL_GetWindowTitle(window);
}


/*set the width of the window*/
void Window::setWidth(uint w){
	SDL_SetWindowSize(window, w, getHeight());
}

/*set the height of the window*/
void Window::setHeight(uint h){
	SDL_SetWindowSize(window, getWidth(), h);
}

/*set the title of the window*/
bool Window::setTitle(std::string s){
	SDL_SetWindowTitle(window, s.c_str());

	return getTitle() == s;
}


/*tells if the window is ready to be used*/
bool Window::isReady(){
	return window != NULL;
}

/*closes the window*/
void Window::close(){
	SDL_DestroyWindow(window);
	window = NULL;
}


/*handle events generated by the window*/
void Window::handleEvents(){

	/*if the window's theme doesn't match the program theme*/
	if(windowDark != darkMode || windowAccent.getUint() != accent->getUint()){
		for(int i=0;i < controls.size();i++){
			controls.at(i)->updateTheme();
		}

		windowDark = darkMode;
		windowAccent = *accent;
	}


	bool eventNeeded = true;

	if(event != NULL){
		/*if the recent event can simple be cleared from the linked-list*/
		if(event->getType() == e_KEYPRESS && event->getKeyState() == s_UP ||
				event->getType() == e_SCROLL ||
				event->getType() == e_TEXTENTRY){
			Event *tail = event->getTail();

			delete event;

			event = tail;
		}
	}


	/*poll events from the queue*/
	SDL_Event e;
	while(SDL_PollEvent(&e)){

		/*close the window if the quit event comes up*/
		if(e.type == SDL_QUIT){
			close();

		}else if(e.type == SDL_WINDOWEVENT){

			if((e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) && sys == WIN){
				fullRedraw = true;
			}

			/*if window resize event*/
			if(e.window.event == SDL_WINDOWEVENT_RESIZED){

				for(int i=0;i<controls.size();i++){
					Control *control = controls.at(i);

					if(control->getLeftLock() && control->getRightLock()){
						bool noResize = true;

						if(control->getLockResize()){
							noResize = !control->setWidth(control->getWidth() + (e.window.data1 - (int)width));
						}

						if(noResize){
							double ratio = (control->getX() + (double)(control->getWidth()/2)) / (double)width;
							control->setX((double)e.window.data1*ratio - (double)(control->getWidth()/2));
						}
					}else if(control->getRightLock()){
						control->setX(control->getX() + (e.window.data1 - (int)width));
					}

					if(control->getTopLock() && control->getBottomLock()){
						bool noResize = true;

						if(control->getLockResize()){
							noResize = !control->setHeight(control->getHeight() + (e.window.data2 - (int)height));
						}

						if(noResize){
							double ratio = (control->getY() + (double)(control->getHeight()/2)) / (double)height;
							control->setY((double)e.window.data2*ratio - (double)(control->getHeight()/2));
						}
					}else if(control->getBottomLock()){
						control->setY(control->getY() + (e.window.data2 - (int)height));
					}
				}

				width = e.window.data1;
				height = e.window.data2;
			}
		}


		if(event != NULL){
			/*if the recent event can simple be cleared from the linked-list*/
			if(event->getType() == e_KEYPRESS && event->getKeyState() == s_UP ||
					event->getType() == e_SCROLL ||
					event->getType() == e_TEXTENTRY){
				Event *tail = event->getTail();

				delete event;

				event = tail;
			}
		}


		/*Mouse Button Down Event*/
		if(e.type == SDL_MOUSEBUTTONDOWN){

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
						event = new Event(e.button.x, e.button.y, b_LEFT, s_DOWN, event);
					}else if(e.button.button == SDL_BUTTON_RIGHT){
						event = new Event(e.button.x, e.button.y, b_RIGHT, s_DOWN, event);
					}else if(e.button.button == SDL_BUTTON_MIDDLE){
						event = new Event(e.button.x, e.button.y, b_MIDDLE, s_DOWN, event);
					}

					break;

				}else if(temp->getType() == e_DRAG){
					
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
							event = new Event(e.button.x, e.button.y, b_LEFT, s_DOWN, event);
						}else if(e.button.button == SDL_BUTTON_RIGHT){
							event = new Event(e.button.x, e.button.y, b_RIGHT, s_DOWN, event);
						}else if(e.button.button == SDL_BUTTON_MIDDLE){
							event = new Event(e.button.x, e.button.y, b_MIDDLE, s_DOWN, event);
						}

						break;

					}else{
						break;
					}

				}else if(temp->getType() == e_CLICK){

					if(e.button.timestamp - temp->getTimeStamp() > 500 ||
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
							event = new Event(e.button.x, e.button.y, b_LEFT, s_DOWN, event);
						}else if(e.button.button == SDL_BUTTON_RIGHT){
							event = new Event(e.button.x, e.button.y, b_RIGHT, s_DOWN, event);
						}else if(e.button.button == SDL_BUTTON_MIDDLE){
							event = new Event(e.button.x, e.button.y, b_MIDDLE, s_DOWN, event);
						}

						break;

					}
				}

				prev = temp;
				temp = temp->getTail();
			}

		/*Mouse Button Up Event*/
		}else if(e.type == SDL_MOUSEBUTTONUP){

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

					if(e.button.timestamp - temp->getTimeStamp() > 500){
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
					event = new Event(e.button.x, e.button.y, click->getButton(), c_DOUBLE, event);
				}else if(click->getClick() == c_DOUBLE){
					event = new Event(e.button.x, e.button.y, click->getButton(), c_TRIPLE, event);
				}else if(click->getClick() == c_TRIPLE){
					event = new Event(e.button.x, e.button.y, click->getButton(), c_SINGLE, event);
				}


				delete click;
				delete press;

			}else if(press){
				event = new Event(e.button.x, e.button.y, press->getButton(), c_SINGLE, event);

				delete press;

			}else{

				if(e.button.button == SDL_BUTTON_LEFT){
					event = new Event(e.button.x, e.button.y, b_LEFT, s_UP, event);
				}else if(e.button.button == SDL_BUTTON_RIGHT){
					event = new Event(e.button.x, e.button.y, b_RIGHT, s_UP, event);
				}else if(e.button.button == SDL_BUTTON_MIDDLE){
					event = new Event(e.button.x, e.button.y, b_MIDDLE, s_UP, event);
				}

			}


		/*Mouse Motion Event*/
		}else if(e.type == SDL_MOUSEMOTION){

			Event *temp = event;
			Event *prev = NULL;
			while(temp){

				if(temp->getType() == e_MOVE || temp->getType() == e_HOVER){

					if(prev != NULL){
						prev->setTail(temp->getTail());
					}else{
						event = temp->getTail();
					}

					event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), event);
					delete temp;
					break;

				}else if(temp->getType() == e_DRAG){

					if(prev != NULL){
						prev->setTail(temp->getTail());
					}else{
						event = temp->getTail();
					}

					event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), temp->getButton(), event);
					delete temp;
					break;

				}else if(temp->getType() == e_HOLD){

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


					if(temp->getControl() == NULL){
						event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), event);
					}else{
						event = new Event(e.motion.x, e.motion.y, temp->getX(), temp->getY(), temp->getButton(), event);
					}

					delete temp;
					break;

				}

				prev = temp;
				temp = temp->getTail();
			}

		/*Mouse Wheel Event*/
		}else if(e.type == SDL_MOUSEWHEEL){
			int x;
			int y;

			uint state = SDL_GetMouseState(&x, &y);

			EVENT_Modifier mod = m_NONE;
			if(SDL_GetModState() & KMOD_SHIFT){
				mod = m_SHIFT;
			}

			if(e.wheel.y > 0){
				event = new Event(x, y, w_UP, mod, event);
			}else if(e.wheel.y < 0){
				event = new Event(x, y, w_DOWN, mod, event);
			}else if(e.wheel.x > 0){
				event = new Event(x, y, w_LEFT, mod, event);
			}else if(e.wheel.x < 0){
				event = new Event(x, y, w_RIGHT, mod, event);
			}

		/*Text Input Event*/
		}else if(e.type == SDL_TEXTINPUT){

			event = new Event(e.text.text, event);

		/*Key Down Event*/
		}else if(e.type == SDL_KEYDOWN){

			if(SDL_GetModState() & KMOD_SHIFT){
				event = new Event(e.key.keysym.sym, s_DOWN, m_SHIFT, event);
			}else if(SDL_GetModState() & KMOD_CTRL){
				event = new Event(e.key.keysym.sym, s_DOWN, m_CONTROL, event);
			}else if(SDL_GetModState() & KMOD_ALT){
				event = new Event(e.key.keysym.sym, s_DOWN, m_ALT, event);
			}else{
				event = new Event(e.key.keysym.sym, s_DOWN, m_NONE, event);
			}

		/*Key Up Event*/
		}else if(e.type == SDL_KEYUP){

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
				event = new Event(e.key.keysym.sym, s_UP, m_SHIFT, event);
			}else if(SDL_GetModState() & KMOD_CTRL){
				event = new Event(e.key.keysym.sym, s_UP, m_CONTROL, event);
			}else if(SDL_GetModState() & KMOD_ALT){
				event = new Event(e.key.keysym.sym, s_UP, m_ALT, event);
			}else{
				event = new Event(e.key.keysym.sym, s_UP, m_NONE, event);
			}

		}else{
			continue;
		}

		if(event != NULL){
			eventNeeded = false;
			handleControls(event);
		}
	}


	/*if an event still needs to be generated*/
	if(eventNeeded){

		Event *temp = event;
		Event *prev = NULL;
		while(temp){

			if(event->getType() == e_HOVER){

				if(prev != NULL){
					prev->setTail(temp->getTail());
				}else{
					event = temp->getTail();
				}

				event = new Event(temp->getX(), temp->getY(), event);
				delete temp;
				break;

			}else if(event->getType() == e_HOLD){	

				if(prev != NULL){
					prev->setTail(temp->getTail());
				}else{
					event = temp->getTail();
				}

				event = new Event(temp->getX(), temp->getY(), temp->getButton(), event);
				delete temp;
				break;

			}else if(temp->getType() == e_MOVE){

				event = new Event(temp->getX(), temp->getY(), event);
				break;

			}else if(temp->getType() == e_DRAG){

				event = new Event(temp->getX(), temp->getY(), temp->getButton(), event);
				break;

			}else if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_DOWN){

				event = new Event(temp->getX(), temp->getY(), temp->getButton(), event);
				break;

			}else if(temp->getType() == e_MOUSEPRESS && temp->getMouseState() == s_UP){
				if(prev != NULL){
					prev->setTail(temp->getTail());
				}else{
					event = temp->getTail();
				}

				event = new Event(temp->getX(), temp->getY(), event);
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

			event = new Event(x, y, event);
		}

		handleControls(event);
	}

}

/*see which controls have a use for the given event*/
void Window::handleControls(Event *event){
	if(activeControl != NULL && activeControl->getVisible()){
		activeControl->handleEvent(event);

		if(event->getControl() == activeControl){
			return;
		}
	}

	for(int i=controls.size()-1;i>=0;i--){

		if(controls.at(i) != activeControl && controls.at(i)->getVisible()){
			controls.at(i)->handleEvent(event);

			if(event->getControl() == controls.at(i)){
				activeControl = controls.at(i);
				return;
			}
		}

	}

}


/*clear the window*/
void Window::clear(Color color){
	SDL_SetRenderDrawColor(renderer, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
	SDL_RenderClear(renderer);
}

/*post the prepared frame to the window*/
void Window::postFrame(){
	SDL_RenderPresent(renderer);
	fullRedraw = false;
}


/*draw the given object*/
void Window::draw(DrawObject *object){
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
void Window::drawControls(){

	for(int i=0;i<controls.size();i++){
		if(controls.at(i) != activeControl && controls.at(i)->getVisible()){
			controls.at(i)->draw(renderer);
		}
	}

	if(activeControl != NULL && activeControl->getVisible()){
		activeControl->draw(renderer);
	}
}


/*update teh theme of the controls in the window*/
bool Window::updateTheme(){
	bool ret = true;

	for(int i=0;i<controls.size();i++){
		ret = ret && controls.at(i)->updateTheme();
	}

	return ret;
}
