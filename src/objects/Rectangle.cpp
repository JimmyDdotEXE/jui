#include "objects/Quadrant.h"
#include "objects/Rectangle.h"
#include <iostream>


/*
	Rectangle constructor

	int x: X position of the rectangle
	int y: Y position of the rectangle
	uint w: width of the rectangle
	uint h: height of the rectangle
	Color color: fill color of the rectangle
 */
Rectangle::Rectangle(int x, int y, uint w, uint h, Color color){
	width = w;
	height = h;
	xPos = x;
	yPos = y;

	fillColor = color;

	outlineColor = Color();
	outlineThickness = 0;
}


/*get the width*/
int Rectangle::getWidth(){
	return width;
}

/*get the height*/
int Rectangle::getHeight(){
	return height;
}

/*get the total width*/
int Rectangle::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Rectangle::getTotalHeight(){
	return getHeight();
}

/*get the thickness of the outline*/
int Rectangle::getOutlineThickness(){
	return outlineThickness;
}

/*get the fill color*/
Color Rectangle::getFillColor(){
	return fillColor;
}

/*get the color of the outline*/
Color Rectangle::getOutlineColor(){
	return outlineColor;
}


/*set the X position*/
bool Rectangle::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Rectangle::setY(double y){
	yPos = y;

	return yPos == y;
}


/*set the width*/
bool Rectangle::setWidth(uint w){
	width = w;

	setTexture(NULL);

	return width == w;
}

/*set the height*/
bool Rectangle::setHeight(uint h){
	height = h;

	setTexture(NULL);

	return height == h;
}

/*set the fill color*/
bool Rectangle::setFillColor(Color color){
	fillColor = color;

	redraw = true;

	return fillColor.getUint() == color.getUint();
}

/*set the outline color*/
bool Rectangle::setOutlineColor(Color color){
	outlineColor = color;

	redraw = true;

	return outlineColor.getUint() == color.getUint();
}

/*set the thickness of the outline*/
bool Rectangle::setOutlineThickness(uint thickness){

	outlineThickness = thickness;

	redraw = true;

	return outlineThickness == thickness;
}



/*set the thickness and color of an outline*/
bool Rectangle::outline(uint thickness, Color color){
	setOutlineThickness(thickness);
	setOutlineColor(color);

	return true;
}


/*free all the memory used*/
void Rectangle::free(){
	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the rectangle*/
bool Rectangle::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}



/*update the texture of the rectangle to be printed later*/
bool Rectangle::updateTexture(SDL_Renderer *renderer){

	if(outlineThickness > 0 && outlineColor.getUint() > 0){
		SDL_SetRenderDrawColor(renderer, outlineColor.getRed(), outlineColor.getGreen(), outlineColor.getBlue(), outlineColor.getAlpha());
		SDL_Rect top = {0, 0, (int)width, (int)outlineThickness};
		SDL_Rect bottom = {0, (int)height - (int)outlineThickness, (int)width, (int)outlineThickness};
		SDL_Rect left = {0, 0, (int)outlineThickness, (int)height};
		SDL_Rect right = {(int)width - (int)outlineThickness, 0, (int)outlineThickness, (int)height};
	
		SDL_RenderFillRect(renderer, &top);
		SDL_RenderFillRect(renderer, &bottom);
		SDL_RenderFillRect(renderer, &left);
		SDL_RenderFillRect(renderer, &right);
	}

	SDL_Rect rect = {(int)outlineThickness, (int)outlineThickness, (int)width - (int)outlineThickness * 2, (int)height - (int)outlineThickness * 2};
	SDL_SetRenderDrawColor(renderer, fillColor.getRed(), fillColor.getGreen(), fillColor.getBlue(), fillColor.getAlpha());
	SDL_RenderFillRect(renderer, &rect);

	return true;
}
