#include <iostream>
#include <algorithm>
#include "controls/ControlGroup.h"


/*
	ControlGroup constructor

	int x: X position
	int y: Y position
	uint w: width
	uint h: height
	std::vector<Control *> c: vector of Control pointers
 */
ControlGroup::ControlGroup(int x, int y, uint w, uint h, std::vector<Control *> c){
	base = new Rectangle(0, 0, w, h);
	base->outline(1);	

	controls = c;

	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int ControlGroup::getWidth(){
	return base->getWidth();
}

/*get the height*/
int ControlGroup::getHeight(){
	return base->getHeight();
}

/*get the total width*/
int ControlGroup::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int ControlGroup::getTotalHeight(){
	return getHeight();
}

bool ControlGroup::getTextureLock(){
	return textureLock;
}


/*set the X position*/
bool ControlGroup::setX(double x){
	for(int i=0;i<controls.size();i++){
		controls.at(i)->setOffsetX(offsetX + x);
	}

	xPos = x;
	return xPos == x;
}

/*set the Y position*/
bool ControlGroup::setY(double y){
	for(int i=0;i<controls.size();i++){
		controls.at(i)->setOffsetY(offsetY + y);
	}

	yPos = y;
	return yPos == y;
}

/*set the width*/
bool ControlGroup::setWidth(uint w){
	for(int i=0;i<controls.size();i++){
		Control *control = controls.at(i);

		if(control->getRightLock() && control->getLeftLock()){
			bool noResize = true;

			if(control->getLockResize()){
				noResize = !control->setWidth(control->getWidth() + ((int)w - getWidth()));
			}

			if(noResize){
				double ratio = (control->getX() + (double)(control->getWidth()/2)) / (double)getWidth();
				control->setX((double)w*ratio - (double)(control->getWidth()/2));
			}

		}else if(control->getRightLock() && !control->getLeftLock()){
			control->setX(control->getX() + ((int)w - getWidth()));
		}
	}

	base->setWidth(w);
	update();
	return getWidth() == w;
}

/*set the height*/
bool ControlGroup::setHeight(uint h){
	for(int i=0;i<controls.size();i++){
		Control *control = controls.at(i);

		if(control->getBottomLock() && control->getTopLock()){
			bool noResize = true;

			if(control->getLockResize()){
				noResize = !control->setHeight(control->getHeight() + ((int)h - getHeight()));
			}

			if(noResize){
				double ratio = (control->getY() + (double)(control->getHeight()/2)) / (double)getHeight();
				control->setY((double)h*ratio - (double)(control->getHeight()/2));
			}

		}else if(control->getBottomLock() && !control->getTopLock()){
			control->setY(control->getY() + ((int)h - getHeight()));
		}
	}

	base->setHeight(h);
	update();
	return getHeight() == h;
}

bool ControlGroup::setTextureLock(bool b){
	textureLock = b;
	return textureLock == b;
}


/*add the given control to the group*/
bool ControlGroup::addControl(Control *control){
	if(std::find(controls.begin(), controls.end(), control) != controls.end()){
		return false;
	}

	control->setOffsetX(getOffsetX() + xPos);
	control->setOffsetY(getOffsetY() + yPos);

	controls.push_back(control);
	return true;
}

/*remove the given control from the group*/
bool ControlGroup::removeControl(Control *control){
	for(int i=0;i < controls.size();i++){
		if(controls.at(i) == control){
			controls.erase(controls.begin() + i);
			return true;
		}
	}

	return false;
}


/*try to handle the given event*/
bool ControlGroup::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	bool ret = false;

	Control *aControl = getActiveControl();

	if(aControl != NULL && aControl->getVisible()){
		aControl->handleEvent(event);

		if(event->getControl() == aControl){
			if(!aControl->getActive()){
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}
	}

	for(int i=controls.size()-1;i>=0;i--){
		if(controls.at(i) != aControl && controls.at(i)->getVisible()){
			controls.at(i)->handleEvent(event);

			if(event->getControl() == controls.at(i)){
				if(controls.at(i)->getActive()){
					setActive(true);
					setActiveControl(controls.at(i));
				}else{
					setActive(false);
				}

				update();
				event->setControl(this);
				ret = true;
			}
		}
	}

	return ret;
}


/*update the theme of the control*/
bool ControlGroup::updateTheme(){
	if(darkMode){
		base->setFillColor(*darkTheme[0]);
		base->setOutlineColor(*darkTheme[2]);
	}else{
		base->setFillColor(*lightTheme[0]);
		base->setOutlineColor(*lightTheme[2]);
	}

	for(int i=0;i<controls.size();i++){
		controls.at(i)->updateTheme();
	}

	redraw = true;
	return true;
}

/*free all the memory used*/
void ControlGroup::free(){
	if(base != NULL){
		base->free();
		delete base;
	}

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the control*/
bool ControlGroup::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void ControlGroup::update(){
	redraw = true;
}

/*update the texture of the control to be printed later*/
bool ControlGroup::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);

	for(int i=0;i<controls.size();i++){
		controls.at(i)->draw(renderer);
	}

	return true;
}
