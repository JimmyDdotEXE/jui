#include <iostream>

#include "General.h"

#include "objects/SemiCircle.h"
#include "objects/Circle.h"


/*
	Circle constructor

	int x: X position of the circle
	int y: Y position of the circle
	uint r: radius of the circle
	Color color: fill color of the circle
 */
Circle::Circle(int x, int y, uint r, Color color){
	xPos = x;
	yPos = y;
	radius = r;
	fillColor = color;
}


/*get the width of the circle*/
int Circle::getWidth(){
	return radius*2;
}

/*get the height of the circle*/
int Circle::getHeight(){
	return radius*2;
}

/*get the total width of the circle*/
int Circle::getTotalWidth(){
	return getWidth();
}

/*get the total height of the circle*/
int Circle::getTotalHeight(){
	return getHeight();
}

/*get the radius of the circle*/
int Circle::getRadius(){
	return radius;
}

/*get the fill color of the circle*/
Color Circle::getFillColor(){
	return fillColor;
}


/*set the X position of the circle*/
bool Circle::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position of the circle*/
bool Circle::setY(double y){
	yPos = y;

	return yPos == y;
}

/*set the width of the circle*/
bool Circle::setWidth(uint w){
	setRadius(w / 2);

	return radius * 2 == w;
}

/*set the height of the circle*/
bool Circle::setHeight(uint h){
	setRadius(h / 2);

	return radius * 2 == h;
}

/*set the radius of the circle*/
bool Circle::setRadius(uint r){
	radius = r;

	setTexture(NULL);

	return radius == r;
}

/*set the fill color of the circle*/
bool Circle::setFillColor(Color color){
	fillColor = color;

	setTexture(NULL);

	return fillColor.getUint() == color.getUint();
}


/*free all the memory used by the circle*/
void Circle::free(){
	setTexture(NULL);
}

/*check if an X and a Y position are within the bounds of the circle*/
bool Circle::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x > xPos && x <= xPos + radius*2 && y > yPos && y <= yPos + radius*2;
}



/*update the texture of the circle to be printed later*/
bool Circle::updateTexture(SDL_Renderer *renderer){

	SemiCircle semi = SemiCircle(0, radius, radius, fillColor);
	semi.setBlendMode(BLEND_NONE);

	semi.draw(renderer);

	semi.setRotation(180);
	semi.setY(0);
	semi.draw(renderer);

	semi.free();
}
