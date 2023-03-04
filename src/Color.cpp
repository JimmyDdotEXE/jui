#include "JUI.h"
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
	if(invertedMode){
		return ~red;
	}else{
		return red;
	}
}

/*get the green value for the color*/
uchar Color::getGreen(){
	if(invertedMode){
		return ~green;
	}else{
		return green;
	}
}

/*get the blue value for the color*/
uchar Color::getBlue(){
	if(invertedMode){
		return ~blue;
	}else{
		return blue;
	}
}

/*get the alpha value for the color*/
uchar Color::getAlpha(){
	return alpha;
}

/*get the numeric value for the color*/
uint Color::getUint(){
	if(BIG_END){
		return (alpha << 24) | (getBlue() << 16) | (getGreen() << 8) | getRed();
	}else{
		return (getRed() << 24) | (getGreen() << 16) | (getBlue() << 8) | alpha;
	}
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
	if(BIG_END){
		alpha = color >> 24;
		blue = color << 8 >> 24;
		green = color << 16 >> 24;
		red = color << 24 >> 24;
	}else{
		red = color >> 24;
		green = color << 8 >> 24;
		blue = color << 16 >> 24;
		alpha = color << 24 >> 24;
	}
}


/*invert the color values for the color*/
void Color::invert(){
	setRed(~getRed());
	setGreen(~getGreen());
	setBlue(~getBlue());
}
