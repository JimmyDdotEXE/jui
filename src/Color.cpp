#include "Color.h"


/*Color constructor using RGBA values*/
Color::Color(uchar r, uchar g, uchar b, uchar a){
	red = r;
	green = g;
	blue = b;
	alpha = a;
}

/*Color constructor using single numeric value*/
Color::Color(uint color){
	setUint(color);
}


/*get the red value for the color*/
uchar Color::getRed(){
	return red;
}

/*get the green value for the color*/
uchar Color::getGreen(){
	return green;
}

/*get the blue value for the color*/
uchar Color::getBlue(){
	return blue;
}

/*get the alpha value for the color*/
uchar Color::getAlpha(){
	return alpha;
}

/*get the numeric value for the color*/
uint Color::getUint(){
	return (red << 24) + (green << 16) + (blue << 8) + alpha;
}


/*set the red value for the color*/
void Color::setRed(uchar r){
	red = r;
}

/*set the green value for the color*/
void Color::setGreen(uchar g){
	green = g;
}

/*set the blue value for the color*/
void Color::setBlue(uchar b){
	blue = b;
}

/*set the alpha value for the color*/
void Color::setAlpha(uchar a){
	alpha = a;
}

/*set the numeric value for the color*/
void Color::setUint(uint color){
	red = color >> 24;
	green = color << 8 >> 24;
	blue = color << 16 >> 24;
	alpha = color << 24 >> 24;
}


/*invert the color values for the color*/
void Color::invert(){
	setUint(~getUint() | 0x000000FF);
}
