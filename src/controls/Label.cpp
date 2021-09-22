#include "controls/Label.h"


/*
	Label constructor

	int x: X position of the Label
	int y: Y position of the Label
	uint s: font size of the Label
	std::string txt: text of the Label
 */
Label::Label(int x, int y, uint s, std::string txt){
	text = new Text(s, txt);
	text->setBlendMode(BLEND_NONE);

	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int Label::getWidth(){
	return text->getWidth();
}

/*get the height*/
int Label::getHeight(){
	return text->getHeight();
}

/*get the total width*/
int Label::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Label::getTotalHeight(){
	return getHeight();
}

/*
	is the control active?
	the function always returns false
*/
bool Label::getActive(){
	return false;
}

/*get the text of the label*/
std::string Label::getText(){
	return text->getString();
}


/*set the X position*/
bool Label::setX(double x){
	text->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Label::setY(double y){
	text->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool Label::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool Label::setHeight(uint h){
	return false;
}

/*set the text of the label*/
bool Label::setText(std::string s){
	text->setString(s);

	redraw = true;
}


/*
	try to handle the given event
	the function does nothing
*/
bool Label::handleEvent(Event *event){
	return false;
}


/*update the theme of the control*/
bool Label::updateTheme(){
	if(darkMode){
		text->setColor(*darkTheme[2]);
	}else{
		text->setColor(*lightTheme[2]);
	}

	redraw = true;
}

/*free all the memory used*/
void Label::free(){
	text->free();

	delete text;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the label*/
bool Label::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
	this function does nothing
*/
void Label::update(){}

/*update the texture of the Label to be printed later*/
bool Label::updateTexture(SDL_Renderer *renderer){
	text->draw(renderer);
}
