#include <iostream>
#include <algorithm>
#include "controls/RadioButton.h"


/*
	RadioButton constructor

	int x: X position of the RadioButton
	int y: Y position of the RadioButton
	std::string lab: label of the RadioButton
 */
RadioButton::RadioButton(int x, int y, std::string lab){
	trim = new Circle(0, 0, 6);
	base = new Circle(1, 1, 5);
	center = new Circle(3, 3, 3);

	text = new Text(12, 0, 12, lab);
	text->setLeftPadding(-1*text->getDescent());

	trim->setBlendMode(BLEND_NONE);
	text->setBlendMode(BLEND_NONE);


	setX(x);
	setY(y);

	selected = false;
	updateTheme();
}


/*get the width*/
int RadioButton::getWidth(){
	return trim->getRadius()*2 + text->getWidth();
}

/*get the height*/
int RadioButton::getHeight(){
	return trim->getRadius()*2;
}

/*get the total width*/
int RadioButton::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int RadioButton::getTotalHeight(){
	return std::max(text->getHeight(), getHeight());
}

bool RadioButton::getSelected(){
	return selected;
}

/*get the label of the RadioButton*/
std::string RadioButton::getLabel(){
	return text->getString();
}

/*get the padding between the button an the label*/
int RadioButton::getPadding(){
	return text->getLeftPadding() + 1;
}


/*set the X position*/
bool RadioButton::setX(double x){
	base->setOffsetX(offsetX + x);
	trim->setOffsetX(offsetX + x);
	center->setOffsetX(offsetX + x);
	text->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool RadioButton::setY(double y){
	base->setOffsetY(offsetY + y);
	trim->setOffsetY(offsetY + y);
	center->setOffsetY(offsetY + y);
	text->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool RadioButton::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool RadioButton::setHeight(uint h){
	return false;
}

bool RadioButton::setSelected(bool b){
	selected = b;

	update();
	return selected == b;
}

/*set the label*/
bool RadioButton::setLabel(std::string lab){
	return text->setString(lab);
}


/*try to handle the given event*/
bool RadioButton::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	if(event->getType() == e_CLICK){

		if(event->getButton() == b_LEFT){

			if(boundsCheck(event->getX(), event->getY()) && !selected){
				setSelected(true);

				event->setControl(this);
			}

		}
	}

	return false;
}


/*update the theme of the control*/
bool RadioButton::updateTheme(){

	if(darkMode){
		base->setFillColor(*darkTheme[1]);
		trim->setFillColor(*darkTheme[2]);
		center->setFillColor(*accent);
		text->setColor(*darkTheme[2]);
	}else{
		base->setFillColor(*lightTheme[1]);
		trim->setFillColor(*lightTheme[2]);
		center->setFillColor(*accent);
		text->setColor(*lightTheme[2]);
	}

	redraw = true;
	return true;
}

/*free all the memory used*/
void RadioButton::free(){
	base->free();
	trim->free();
	center->free();
	text->free();

	delete base;
	delete trim;
	delete center;
	delete text;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the RadioButton*/
bool RadioButton::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void RadioButton::update(){
	redraw = true;
}

/*update the texture of the RadioButton to be printed later*/
bool RadioButton::updateTexture(SDL_Renderer *renderer){
	trim->draw(renderer);
	base->draw(renderer);

	text->draw(renderer);

	if(selected){
		center->draw(renderer);
	}

	return true;
}
