#include "objects/Rectangle.h"
#include "objects/Line.h"


/*
	Line constructor

	int x: X position of the line
	int y: Y position of the line
	uint len: length of the line
	Color color: color of the line
	e_Orien orien: orientation of the line
 */
Line::Line(int x, int y, uint len, Color colo, e_Orien orien){
	orientation = orien;
	length = len;
	color = colo;
	xPos = x;
	yPos = y;
}


/*
	get the width
	width is 1 if vertical or same as the length if horizontal
*/
int Line::getWidth(){
	if(orientation == HORIZONTAL){
		return length;
	}else{
		return 1;
	}
}

/*
	get the height
	height is 1 if horizontal or same as the length if vertical
*/
int Line::getHeight(){
	if(orientation == VERTICAL){
		return length;
	}else{
		return 1;
	}
}

/*get the total width*/
int Line::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Line::getTotalHeight(){
	return getHeight();
}

/*get the line length*/
int Line::getLength(){
	return length;
}

/*get the line color*/
Color Line::getColor(){
	return color;
}


/*set the X position*/
bool Line::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Line::setY(double y){
	yPos = y;

	return yPos == y;
}


/*
	set the width
	this function always fails
*/
bool Line::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool Line::setHeight(uint h){
	return false;
}

/*set the line length*/
bool Line::setLength(uint len){
	length = len;

	return length == len;
}

/*set the color of the line*/
bool Line::setColor(Color colo){
	color = colo;

	return color.getUint() == colo.getUint();
}


/*rotates the line 90 degrees to the opposite orientation*/
void Line::rotate(){
	if(orientation == HORIZONTAL){
		orientation = VERTICAL;
	}else{
		orientation = HORIZONTAL;
	}
}


/*free all the memory used*/
void Line::free(){
	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the line*/
bool Line::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	if(orientation == HORIZONTAL && x > xPos && x <= xPos + length && y == yPos){
		return true;
	}else if(orientation == VERTICAL && x == xPos && y == yPos && y <= yPos + length){
		return true;
	}else{
		return false;
	}
}



/*update the texture of the line to be printed later*/
bool Line::updateTexture(SDL_Renderer *renderer){
		SDL_SetRenderDrawColor(renderer, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
		SDL_RenderClear(renderer);
}
