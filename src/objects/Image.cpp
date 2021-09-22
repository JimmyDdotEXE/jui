#include <iostream>
#include <SDL2/SDL_image.h>

#include "objects/Image.h"


#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	uint rmask = 0x000000ff;
	uint gmask = 0x0000ff00;
	uint bmask = 0x00ff0000;
	uint amask = 0xff000000;
#else
	uint rmask = 0xff000000;
	uint gmask = 0x00ff0000;
	uint bmask = 0x0000ff00;
	uint amask = 0x000000ff;
#endif


/*
	Image constructor

	int x: X position of the image
	int y: Y position of the image
	std::string fileName: location of the file to load
 */
Image::Image(int x, int y, std::string fileName){
	image = NULL;

	if(load(fileName)){
		width = image->w;
		height = image->h;
		scale = 1;
		xPos = x;
		yPos = y;
	}
}

/*
	Image constructor

	int x: X position of the image
	int y: Y position of the image
	uint w: width of the image
	uint h: height of the image
	Color color: color to fill the image with
 */
Image::Image(int x, int y, uint w, uint h, Color color){
	image = NULL;

	if(image = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask)){
		SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);

		SDL_FillRect(image, NULL, SDL_MapRGBA(image->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
	
		file = "";
		width = w;
		height = h;
		scale = 1;
		xPos = x;
		yPos = y;
	}
}


/*get the width*/
int Image::getWidth(){
	return width;
}

/*get the height*/
int Image::getHeight(){
	return height;
}

/*get the total width*/
int Image::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Image::getTotalHeight(){
	return getHeight();
}

/*
	get the scale of the image
	scale isn't currently used for anything
*/
int Image::getScale(){
	return scale;
}

/*get the color value of the pixel at the given coordinates*/
Color Image::getPixel(uint x, uint y){
	if(image){
		uint *pixels = (uint *)image->pixels;
		return Color(pixels[(y * image->w) + x]);
	}else{
		return Color(0);
	}
}


/*set the X position*/
bool Image::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Image::setY(double y){
	yPos = y;

	return yPos == y;
}

/*set the width*/
bool Image::setWidth(uint w){
	return false;
}

/*set the height*/
bool Image::setHeight(uint h){
	return false;
}

/*set the color value of the pixel at the given coordinates*/
bool Image::setPixel(uint x, uint y, Color color){
	if(image){
		uint *pixels = (uint *)image->pixels;
		pixels[(y * image->w) + x] = color.getUint();

		redraw = true;

		return true;
	}else{
		return false;
	}
}

/*
	set the scale of the image
	scale isn't currently used for anything
*/
bool Image::setScale(uint s){
	if(s > 0)
		scale = s;
}


/*resizing the image doesn't work yet*/
bool Image::resize(uint w, uint h, int xOff, int yOff, uint fillColor){
	return false;
}


/*load an image from the given location*/
bool Image::load(std::string fileName){
	free();

	if(image = IMG_Load(fileName.c_str())){
		file = fileName;

		return true;
	}else{
		return false;
	}
}


/*free all the memory used*/
void Image::free(){
	SDL_FreeSurface(image);

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the image*/
bool Image::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + width*scale && y >= yPos && y < yPos + height*scale;
}



/*update the texture of the image to be printed later*/
bool Image::updateTexture(SDL_Renderer *renderer){
	if(renderer && image){

		SDL_Texture *temp = SDL_CreateTextureFromSurface(renderer, image);
		SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_NONE);

		SDL_RenderCopy(renderer, temp, NULL, NULL);

		SDL_DestroyTexture(temp);
	}
}
