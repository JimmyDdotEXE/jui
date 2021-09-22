#include <iostream>
#include "controls/Toggle.h"


/*
	Toggle constructor

	int x: X position of the toggle
	int y: Y position of the toggle
	bool *a: pointer to the boolean the toggle controls
 */
Toggle::Toggle(int x, int y, bool *a){
	baseLeft = new SemiCircle(0, 0, 10);
	baseLeft->setRotation(90);
	baseMid = new Rectangle(baseLeft->getRadius(), 0, baseLeft->getRadius()*2, baseLeft->getRadius()*2);
	baseRight = new SemiCircle(baseMid->getX() + baseMid->getWidth(), 0, 10);
	baseRight->setRotation(270);

	slider = new Circle(2, 2, baseLeft->getRadius()-2);

	baseLeft->setBlendMode(BLEND_NONE);
	baseRight->setBlendMode(BLEND_NONE);

	state = a;

	setX(x);
	setY(y);

	update();
}


/*get the width*/
int Toggle::getWidth(){
	return baseLeft->getRadius() + baseMid->getWidth() + baseRight->getRadius();
}

/*get the height*/
int Toggle::getHeight(){
	return baseMid->getHeight();
}

/*get the total width*/
int Toggle::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Toggle::getTotalHeight(){
	return getHeight();
}

/*
	is the control active?
	the function always returns false
*/
bool Toggle::getActive(){
	return false;
}

/*get the bool pointer that the toggle controls*/
bool *Toggle::getState(){
	return state;
}


/*set the X position*/
bool Toggle::setX(double x){
	baseLeft->setOffsetX(offsetX + x);
	baseMid->setOffsetX(offsetX + x);
	baseRight->setOffsetX(offsetX + x);
	slider->setOffsetX(offsetX + x);


	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Toggle::setY(double y){
	baseLeft->setOffsetY(offsetY + y);
	baseMid->setOffsetY(offsetY + y);
	baseRight->setOffsetY(offsetY + y);
	slider->setOffsetY(offsetY + y);


	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool Toggle::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool Toggle::setHeight(uint h){
	return false;
}

/*set if the cotrol is in focus*/
bool Toggle::setFocus(bool b){
	focus = b;

	return focus == b;
}

/*set bool pointer that the toggle controls*/
bool Toggle::setState(bool *a){
	state = a;

	update();

	return state == a;
}


/*try to handle the given event*/
bool Toggle::handleEvent(Event *event){

	if(event->getType() == e_MOUSEPRESS && event->getMouseState() == s_DOWN){

		//if the mouse click happens within the bounds of the button
		if(boundsCheck(event->getX(), event->getY()) && state != NULL){
				setFocus(true);

				event->setControl(this);

		}

	//if the event is the release of a mouse click while the button is active
	}else if(event->getType() == e_CLICK && focus){

		setFocus(false);

		*state = !*state;

		update();

	//if the event is a mouse drag while the button is active
	}else if(event->getType() == e_DRAG && focus){

			if(focus){
				setFocus(false);
			}


	}

	return false;
}


/*update the theme of the control*/
bool Toggle::updateTheme(){
	if(darkMode){
		if(state != NULL){

			if(*state){
				baseLeft->setFillColor(*accent);
				baseMid->setFillColor(*accent);
				baseRight->setFillColor(*accent);
			}else{
				baseLeft->setFillColor(*darkTheme[4]);
				baseMid->setFillColor(*darkTheme[4]);
				baseRight->setFillColor(*darkTheme[4]);
			}

		}else{
			baseLeft->setFillColor(*darkTheme[4]);
			baseMid->setFillColor(*darkTheme[4]);
			baseRight->setFillColor(*darkTheme[4]);
		}

		slider->setFillColor(*darkTheme[3]);
	}else{
		if(state != NULL){

			if(*state){
				baseLeft->setFillColor(*accent);
				baseMid->setFillColor(*accent);
				baseRight->setFillColor(*accent);
			}else{
				baseLeft->setFillColor(*lightTheme[4]);
				baseMid->setFillColor(*lightTheme[4]);
				baseRight->setFillColor(*lightTheme[4]);
			}

		}else{
			baseLeft->setFillColor(*lightTheme[4]);
			baseMid->setFillColor(*lightTheme[4]);
			baseRight->setFillColor(*lightTheme[4]);
		}

		slider->setFillColor(*lightTheme[3]);
	}

	redraw = true;
}

/*free all the memory used*/
void Toggle::free(){
	baseLeft->free();
	baseRight->free();
	baseMid->free();
	slider->free();

	delete baseLeft;
	delete baseRight;
	delete baseMid;
	delete slider;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the toggle*/
bool Toggle::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void Toggle::update(){
	if(state != NULL){
		if(*state){
			slider->setX(baseMid->getX() + baseRight->getRadius() + (baseRight->getRadius() - slider->getRadius()));
		}else{
			slider->setX(baseLeft->getX() + (baseLeft->getRadius() - slider->getRadius()));
		}
	}else{
		slider->setX(baseLeft->getX() + (baseLeft->getRadius() - slider->getRadius()));
	}

	updateTheme();

	redraw = true;
}

/*update the texture of the toggle to be printed later*/
bool Toggle::updateTexture(SDL_Renderer *renderer){

	baseLeft->draw(renderer);
	baseRight->draw(renderer);
	baseMid->draw(renderer);

	slider->draw(renderer);

}
