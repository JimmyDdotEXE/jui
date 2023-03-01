#include "controls/Ribbon.h"
#include "controls/RibbonOption.h"


RibbonOption::RibbonOption(std::string label, ContextMenu *menu) : Button(0 , 0, label){
	subMenu = menu;
	selected = false;
	hoverMode = NULL;

	hasLeftClick = true;

	base->outline(0, Color());

	updateTheme();
}


int RibbonOption::getTotalWidth(){
	if(subMenu != NULL && subMenu->getTotalWidth() >= getWidth()){
		return subMenu->getTotalWidth();
	}

	return getWidth();
}

int RibbonOption::getTotalHeight(){
	if(subMenu != NULL){
		return getHeight() + subMenu->getTotalHeight();
	}

	return getHeight();
}

bool RibbonOption::getSelected(){
	return selected;
}


bool RibbonOption::setX(double x){
	if(subMenu != NULL){
		subMenu->setOffsetX(offsetX + x);
	}

	return Button::setX(x);
}

bool RibbonOption::setY(double y){
	if(subMenu != NULL){
		subMenu->setOffsetY(offsetY + y);
	}

	return Button::setY(y);
}

bool RibbonOption::setWidth(uint w){
	return false;
}

bool RibbonOption::setHeight(uint h){
	return false;
}

bool RibbonOption::setSelected(bool b){
	if(b == false && subMenu != NULL){
		subMenu->collapse();
	}

	selected = b;
	update();
	return selected == b;
}

bool RibbonOption::setHoverMode(bool *b){
	if(subMenu != NULL){
		subMenu->setHoverMode(b);
	}

	hoverMode = b;
	return hoverMode == b;
}


bool RibbonOption::leftClick(){
	*hoverMode = !selected;
	return setSelected(!selected);
}


bool RibbonOption::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	if(event->getType() == e_MOVE && *hoverMode){
		if(boundsCheck(event->getX(), event->getY()) && !selected){
			setSelected(true);
			setActive(false);
			event->setControl(this);
			return true;
		}
	}else if(event->getType() == e_HOVER && *hoverMode){
		if(boundsCheck(event->getX(), event->getY()) && !selected){
			setSelected(true);
			setActive(false);
			event->setControl(this);
			return true;
		}
	}else if(event->getType() == e_CLICK){
		if(boundsCheck(event->getX(), event->getY()) && leftClickAvailable()){
			if(leftClick()){
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


bool RibbonOption::updateTheme(){
	if(darkMode){
		if(subMenu != NULL){
			subMenu->updateTheme();
		}

		if(selected){
			base->setFillColor(*accent);
			text->setColor(*darkTheme[1]);
		}else{
			base->setFillColor(*darkTheme[1]);
			text->setColor(*darkTheme[4]);
		}
	}else{
		if(subMenu != NULL){
			subMenu->updateTheme();
		}

		if(selected){
			base->setFillColor(*accent);
			text->setColor(*lightTheme[1]);
		}else{
			base->setFillColor(*lightTheme[1]);
			text->setColor(*lightTheme[4]);
		}
	}

	redraw = true;
	return true;
}

void RibbonOption::free(){
	if(subMenu != NULL){
		subMenu->free();
		delete subMenu;
		subMenu = NULL;
	}

	Button::free();
}

bool RibbonOption::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




void RibbonOption::update(){
	updateTheme();
}

bool RibbonOption::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);
	text->draw(renderer);

	if(subMenu != NULL && selected){
		subMenu->setX(0);
		subMenu->setY(getHeight());
		subMenu->draw(renderer);
	}

	return true;
}
