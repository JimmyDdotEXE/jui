#include <iostream>
#include <cmath>

#include "Utility.h"

#include "objects/Image.h"
#include "objects/Quadrant.h"


/*
	Quadrant constructor

	int x: X position of the SemiCircle
	int y: Y position of the SemiCircle
	uint r: radius of the SemiCircle
	Color color: fill color of the SemiCircle
 */
Quadrant::Quadrant(int x, int y, uint r, Color color){
	xPos = x;
	yPos = y;
	radius = r;
	fillColor = color;
}


/*get the width*/
int Quadrant::getWidth(){
	return radius;
}

/*get the height*/
int Quadrant::getHeight(){
	return radius;
}

/*get the total width*/
int Quadrant::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Quadrant::getTotalHeight(){
	return getHeight();
}

/*get the radius*/
int Quadrant::getRadius(){
	return radius;
}

/*get the fill color*/
Color Quadrant::getFillColor(){
	return fillColor;
}


/*set the X position*/
bool Quadrant::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Quadrant::setY(double y){
	yPos = y;

	return yPos == y;
}


/*set the width*/
bool Quadrant::setWidth(uint w){
	setRadius(w);

	return radius == w;
}

/*set the height*/
bool Quadrant::setHeight(uint h){
	setRadius(h);

	return radius == h;
}

/*set the radius*/
bool Quadrant::setRadius(uint r){
	radius = r;

	setTexture(NULL);

	return radius == r;
}

/*set the fill color*/
bool Quadrant::setFillColor(Color color){
	fillColor = color;

	setTexture(NULL);

	return fillColor.getUint() == color.getUint();
}


/*free all the memory used*/
void Quadrant::free(){
	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the Quadrant*/
bool Quadrant::boundsCheck(int x, int y){
	x -= offsetX;
	y -= offsetY;

	return x > xPos && x <= xPos + getWidth() && y > yPos && y <= yPos + getHeight();
}



/*update the texture of the Quadrant to be printed later*/
bool Quadrant::updateTexture(SDL_Renderer *renderer){
	Image im = Image(0, 0, radius, radius, Color());
	im.setBlendMode(BLEND_NONE);

	if(radius > 0){
		int centerX = 0;
		int centerY = 0;

		int x=centerX;
		int y=centerY+radius;


		while(y >= 0){

			if(x > y || !(std::pow(x, 2) + std::pow(y, 2) < std::pow(radius, 2))){
				x = 0;
				y--;
			}else{
				if(std::pow(x+1, 2) + std::pow(y+1, 2) <= std::pow(radius, 2)){
					im.setPixel(x, y, fillColor);
					im.setPixel(y, x, fillColor);
				}else if(y >= (int)(radius - (radius * (1 - cos(M_PI/4))))){
					Color fill = fillColor;


					double area = 0;
					for(double i=.1;i<=1.0;i+=.1){
						double seg = (std::sqrt(std::pow(radius, 2) - std::pow(x + i, 2)) - y) * .1;
						if(seg > 0){
							area += seg;
						}else{
							break;
						}
					}

					if(area > 1){
						area = 1;
					}

					fill.setAlpha(255 * area);

					im.setPixel(x, y, fill);
					im.setPixel(y, x, fill);
				}

				x++;
			}
		}


		im.draw(renderer);
	}

	im.free();
	return true;
}
