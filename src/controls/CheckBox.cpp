#include <algorithm>
#include "controls/CheckBox.h"


/*
	CheckBox constructor

	int x: X position of the CheckBox
	int y: Y position of the CheckBox
	std::string lab: label of the CheckBox
 */
CheckBox::CheckBox(int x, int y, std::string lab){
	base = new Rectangle(0, 0, 12, 12);
	base->setOutlineThickness(1);

	center = new Rectangle(3, 3, 6, 6);

	text = new Text(12, 0, 12, lab);
	text->setLeftPadding(-1*text->getDescent());

	text->setBlendMode(BLEND_NONE);


	setX(x);
	setY(y);

	selected = false;

	updateTheme();
}


/*get the width*/
int CheckBox::getWidth(){
	return base->getWidth() + text->getWidth();
}

/*get the height*/
int CheckBox::getHeight(){
	return base->getHeight();
}

/*get the total width*/
int CheckBox::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int CheckBox::getTotalHeight(){
	return std::max(text->getHeight(), getHeight());
}

bool CheckBox::getSelected(){
	return selected;
}

/*get the label of the CheckBox*/
std::string CheckBox::getLabel(){
	return text->getString();
}

/*get the padding between the button an the label*/
int CheckBox::getPadding(){
	return text->getLeftPadding() + 1;
}


/*set the X position*/
bool CheckBox::setX(double x){
	base->setOffsetX(offsetX + x);
	center->setOffsetX(offsetX + x);
	text->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool CheckBox::setY(double y){
	base->setOffsetY(offsetY + y);
	center->setOffsetY(offsetY + y);
	text->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool CheckBox::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool CheckBox::setHeight(uint h){
	return false;
}

bool CheckBox::setSelected(bool b){
	selected = b;

	update();
	return selected == b;
}

/*set the label*/
bool CheckBox::setLabel(std::string lab){
	return text->setString(lab);
}


/*try to handle the given event*/
bool CheckBox::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	if(event->getType() == e_CLICK){

		if(event->getButton() == b_LEFT){

			if(boundsCheck(event->getX(), event->getY())){
				setSelected(!selected);

				event->setControl(this);
			}

		}
	}

	return false;
}


/*update the theme of the control*/
bool CheckBox::updateTheme(){

	if(darkMode){
		base->setFillColor(*darkTheme[1]);
		base->setOutlineColor(*darkTheme[2]);
		center->setFillColor(*accent);
		text->setColor(*darkTheme[2]);
	}else{
		base->setFillColor(*lightTheme[1]);
		base->setOutlineColor(*lightTheme[2]);
		center->setFillColor(*accent);
		text->setColor(*lightTheme[2]);
	}

	redraw = true;
	return true;
}

/*free all the memory used*/
void CheckBox::free(){
	base->free();
	center->free();
	text->free();

	delete base;
	delete center;
	delete text;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the CheckBox*/
bool CheckBox::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void CheckBox::update(){
	redraw = true;
}

/*update the texture of the CkeckBox to be printed later*/
bool CheckBox::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);

	text->draw(renderer);

	if(selected){
		center->draw(renderer);
	}

	return true;
}
