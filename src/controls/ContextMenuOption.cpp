#include <iostream>
#include "Window.h"
#include "controls/ContextMenu.h"
#include "controls/ContextMenuOption.h"


ContextMenuOption::ContextMenuOption(std::string label, std::function<bool()> onLeftClick, std::function<bool()> available) : Button(0 , 0, label, onLeftClick=onLeftClick){
	subMenu = NULL;
	arrow = NULL;
	selected = false;
	hoverMode = NULL;

	availabilityCheck = available;

	base->outline(0, Color());

	updateTheme();
}

ContextMenuOption::ContextMenuOption(std::string label, ContextMenu *menu, std::function<bool()> available) : Button(0 , 0, label){
	subMenu = menu;
	selected = false;
	hoverMode = NULL;

	availabilityCheck = available;

	arrow = new Icon(text->getHeight()/2, text->getHeight());

	for(int y=0;y<arrow->getWidth();y++){
		for(int x=0;x<y;x++){
			arrow->setPixel(x, y, Color(0, 0, 0, 255));
			arrow->setPixel(x, text->getHeight()-y-1, Color(0, 0, 0, 255));
		}
	}

	arrow->setY(text->getY());
	arrow->setX(text->getX()+text->getWidth());

	setWidth(arrow->getX()+arrow->getWidth()+text->getRightPadding());

	base->outline(0, Color());

	updateTheme();
}


int ContextMenuOption::getTotalWidth(){
	if(subMenu != NULL){
		return getWidth() + subMenu->getTotalWidth();
	}

	return getWidth();
}

int ContextMenuOption::getTotalHeight(){
	if(subMenu != NULL){
		return subMenu->getTotalHeight();
	}

	return getHeight();
}

bool ContextMenuOption::getSelected(){
	return selected;
}


bool ContextMenuOption::setX(double x){
	if(subMenu != NULL){
		subMenu->setOffsetX(offsetX + x);
		arrow->setOffsetX(offsetX + x);
	}

	return Button::setX(x);
}

bool ContextMenuOption::setY(double y){
	if(subMenu != NULL){
		subMenu->setOffsetY(offsetY + y);
		arrow->setOffsetY(offsetY + y);
	}

	return Button::setY(y);
}

bool ContextMenuOption::setWidth(uint w){
	if(arrow != NULL){
		arrow->setX(w-arrow->getWidth()-text->getRightPadding());
	}

	if(subMenu != NULL){
		subMenu->setX(w);
	}

	return base->setWidth(w);
}

bool ContextMenuOption::setHeight(uint h){
	return false;
}

bool ContextMenuOption::setSelected(bool b){
	if(b == false && subMenu != NULL){
		subMenu->collapse();
	}

	selected = b;
	update();
	return selected == b;
}

bool ContextMenuOption::setHoverMode(bool *b){
	if(subMenu != NULL){
		subMenu->setHoverMode(b);
	}

	hoverMode = b;
	return hoverMode == b;
}


bool ContextMenuOption::checkAvailability(){
	if(availabilityCheck != NULL){
		return availabilityCheck();
	}

	return true;;
}


bool ContextMenuOption::handleEvent(Event *event){
	if(event->getType() == e_MOVE){
		if(boundsCheck(event->getX(), event->getY()) && !selected){
			setSelected(true);
			setActive(true);
			update();
			event->setControl(this);
			return true;
		}
	}else if(event->getType() == e_HOVER){
		if(boundsCheck(event->getX(), event->getY()) && !selected){
			setSelected(true);
			setActive(true);
			update();
			event->setControl(this);
			return true;
		}
	}else if(event->getType() == e_CLICK && checkAvailability()){
		if(boundsCheck(event->getX(), event->getY()) && leftClickAvailable()){
			if(leftClick()){
				if(hoverMode != NULL){
					*hoverMode = false;
				}

				setSelected(false);
				setActive(false);
				update();
				event->setControl(this);
				return true;
			}
		}
	}else if(event->getType() == e_MOUSEPRESS && event->getMouseState() == s_DOWN){
		if(boundsCheck(event->getX(), event->getY())){
			setActive(true);
			event->setControl(this);
			return true;
		}
	}

	if(selected && subMenu != NULL){
		subMenu->handleEvent(event);

		if(event->getControl() == subMenu){
			event->setControl(this);
			update();
			return true;
		}
	}

	return false;
}


bool ContextMenuOption::updateTheme(){
	if(darkMode){
		if(subMenu != NULL){
			subMenu->updateTheme();
		}

		if(!checkAvailability()){
			base->setFillColor(*darkTheme[1]);
			text->setColor(*darkTheme[3]);

			if(arrow != NULL){
				arrow->setColor(*darkTheme[3]);
			}
		}else if(selected){
			base->setFillColor(*accent);
			text->setColor(*darkTheme[1]);

			if(arrow != NULL){
				arrow->setColor(*darkTheme[1]);
			}
		}else{
			base->setFillColor(*darkTheme[1]);
			text->setColor(*darkTheme[4]);

			if(arrow != NULL){
				arrow->setColor(*darkTheme[4]);
			}
		}
	}else{
		if(subMenu != NULL){
			subMenu->updateTheme();
		}

		if(!checkAvailability()){
			base->setFillColor(*lightTheme[1]);
			text->setColor(*lightTheme[3]);

			if(arrow != NULL){
				arrow->setColor(*lightTheme[3]);
			}
		}else if(selected){
			base->setFillColor(*accent);
			text->setColor(*lightTheme[1]);

			if(arrow != NULL){
				arrow->setColor(*lightTheme[1]);
			}
		}else{
			base->setFillColor(*lightTheme[1]);
			text->setColor(*lightTheme[4]);

			if(arrow != NULL){
				arrow->setColor(*lightTheme[4]);
			}
		}
	}

	redraw = true;
	return true;
}

void ContextMenuOption::free(){
	if(subMenu != NULL){
		subMenu->free();
		delete subMenu;
		subMenu = NULL;
	}

	if(arrow != NULL){
		arrow->free();
		delete arrow;
		arrow = NULL;
	}

	Button::free();
}

bool ContextMenuOption::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}



void ContextMenuOption::update(){
	updateTheme();
}

bool ContextMenuOption::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);
	text->draw(renderer);

	if(subMenu != NULL){
		arrow->draw(renderer);

		if(selected && checkAvailability()){
			subMenu->setX(getWidth());
			subMenu->setY(0);
			subMenu->draw(renderer);
		}
	}

	return true;
}
