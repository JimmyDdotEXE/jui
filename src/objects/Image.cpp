#include <iostream>
#include <SDL2/SDL_image.h>
#include "objects/Image.h"


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

		if(invertedMode){
			reload();
		}
	}

	fresh = true;
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

	if(image = SDL_CreateRGBSurface(0, w, h, 32, R_MASK, G_MASK, B_MASK, A_MASK)){
		SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);

		SDL_FillRect(image, NULL, SDL_MapRGBA(image->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));
	
		file = "";
		width = w;
		height = h;
		scale = 1;
		xPos = x;
		yPos = y;
	}

	fresh = true;
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
	return width*scale;
}

/*get the total height*/
int Image::getTotalHeight(){
	return height*scale;
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
	if(x >= getWidth() || y >= getHeight()){
		return Color(0);
	}else if(image){
		uint p;
		int bpp = image->format->BytesPerPixel;
		Uint8 *pixel = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

		uchar red;
		uchar green;
		uchar blue;
		uchar alpha;

		switch(bpp){
			case 1:
				SDL_GetRGBA(*pixel, image->format, &red, &green, &blue, &alpha);
				break;
			case 2:
				SDL_GetRGBA(*(Uint16 *)pixel, image->format, &red, &green, &blue, &alpha);
				break;
			case 3:
				if(BIG_END){
					red = pixel[0];
					green = pixel[1];
					blue = pixel[2];
				}else{
					blue = pixel[0];
					green = pixel[1];
					red = pixel[2];
				}

				alpha = 255;
				break;
			case 4:
				SDL_GetRGBA(*(Uint32 *)pixel, image->format, &red, &green, &blue, &alpha);
				break;
			default:
				return Color(0);
		}


		if((invertedMode && !inversionNeeded) || (!invertedMode && inversionNeeded)){
			return Color(~red, ~green, ~blue, alpha);
		}else{
			return Color(red, green, blue, alpha);
		}
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
		int bpp = image->format->BytesPerPixel;
		Uint8 *pixel = (Uint8 *)image->pixels + y * image->pitch + x * bpp;

		switch(bpp){
			case 1:
				*pixel = SDL_MapRGBA(image->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
				break;
			case 2:
				*(Uint16 *)pixel = SDL_MapRGBA(image->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
				break;
			case 3:
				if(BIG_END){
					pixel[0] = color.getRed();
					pixel[1] = color.getGreen();
					pixel[2] = color.getBlue();
				}else{
					pixel[0] = color.getBlue();
					pixel[1] = color.getGreen();
					pixel[2] = color.getRed();
				}
				break;
			case 4:
				*(Uint32 *)pixel = SDL_MapRGBA(image->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
				break;
		}

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
	scale = s;
	redraw = true;
	return scale == s;
}


bool Image::resize(uint w, uint h, int xOff, int yOff, Color color){
	SDL_Surface *oldImage = image;

	if(image = SDL_CreateRGBSurface(0, w, h, 32, R_MASK, G_MASK, B_MASK, A_MASK)){
		SDL_SetSurfaceBlendMode(image, SDL_BLENDMODE_BLEND);

		SDL_FillRect(image, NULL, SDL_MapRGBA(image->format, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha()));

		SDL_Rect dest = {xOff, yOff, (int)width, (int)height};
		SDL_BlitSurface(oldImage, NULL, image, &dest);

		SDL_FreeSurface(oldImage);

		file = "";
		width = w;
		height = h;

		redraw = true;
		return true;
	}else{
		image = oldImage;
		return false;
	}
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

bool Image::reload(){
	for(int x=0;x<width;x++){
		for(int y=0;y<height;y++){
			setPixel(x, y, getPixel(x, y));
		}
	}

	return true;
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
	if(inversionNeeded && !fresh){
		reload();
	}

	if(renderer && image){

		SDL_Texture *temp = SDL_CreateTextureFromSurface(renderer, image);
		SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_NONE);

		SDL_RenderCopy(renderer, temp, NULL, NULL);

		SDL_DestroyTexture(temp);
	}

	fresh = false;
	return true;
}
