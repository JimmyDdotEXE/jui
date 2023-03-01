#include <iostream>

#include "Utility.h"

#include "objects/Quadrant.h"
#include "objects/SemiCircle.h"


/*
	SemiCircle constructor

	int x: X position of the SemiCircle
	int y: Y position of the SemiCircle
	uint r: radius of the SemiCircle
	Color color: fill color of the SemiCircle
 */
SemiCircle::SemiCircle(int x, int y, uint r, Color color){
	xPos = x;
	yPos = y;
	radius = r;
	fillColor = color;
}


/*get the width*/
int SemiCircle::getWidth(){
	return radius*2;
}

/*get the height*/
int SemiCircle::getHeight(){
	return radius;
}

/*get the total width*/
int SemiCircle::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int SemiCircle::getTotalHeight(){
	return getHeight();
}

/*get the radius*/
int SemiCircle::getRadius(){
	return radius;
}

/*get the fill color*/
Color SemiCircle::getFillColor(){
	return fillColor;
}


/*set the X position*/
bool SemiCircle::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool SemiCircle::setY(double y){
	yPos = y;

	return yPos == y;
}

/*set the width*/
bool SemiCircle::setWidth(uint w){
	setRadius(w*2);

	return radius == w*2;
}

/*set the height*/
bool SemiCircle::setHeight(uint h){
	setRadius(h);

	return radius == h;
}

/*set the radius*/
bool SemiCircle::setRadius(uint r){
	radius = r;

	setTexture(NULL);

	return radius == r;
}

/*set the fill color*/
bool SemiCircle::setFillColor(Color color){
	fillColor = color;

	setTexture(NULL);

	return fillColor.getUint() == color.getUint();
}


/*free all the memory used*/
void SemiCircle::free(){
	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the SemiCircle*/
bool SemiCircle::boundsCheck(int x, int y){
	x -= offsetX;
	y -= offsetY;

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}



/*update the texture of the SemiCircle to be printed later*/
bool SemiCircle::updateTexture(SDL_Renderer *renderer){
	Quadrant quad = Quadrant(radius, 0, radius, fillColor);
	quad.setBlendMode(BLEND_NONE);

	quad.draw(renderer);

	quad.setRotation(90);
	quad.setX(0);
	quad.draw(renderer);

	quad.free();
	return true;
}
