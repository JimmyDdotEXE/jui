#include <iostream>
#include <cmath>
#include "objects/Rectangle.h"
#include "objects/DrawObject.h"


/*
	DrawObject constructor
	initializes all DrawObject variables
*/
DrawObject::DrawObject(){
	restrict = NULL;
	texture = NULL;

	xPos = 0;
	yPos = 0;

	offsetX = 0;
	offsetY = 0;

	rotation = 0;
	flip = SDL_FLIP_NONE;

	blend = BLEND_ALPHA;

	redraw = false;
	textureLock = true;

	lastRender = 0;
}


/*get the X position of the object*/
double DrawObject::getX(){
	return xPos;
}

/*get the Y position of the object*/
double DrawObject::getY(){
	return yPos;
}

/*
	getOffsetX function
	returns the x offset of the object
*/
int DrawObject::getOffsetX(){
	return offsetX;
}

/*
	getOffsetY function
	returns the y offset of the object
*/
int DrawObject::getOffsetY(){
	return offsetY;
}

/*
	getRotation function
	returns the angle of rotation for the object
	angle of rotation is represented in degrees
*/
double DrawObject::getRotation(){
	return rotation;
}

/*getFlip function*/
SDL_RendererFlip DrawObject::getFlip(){
	return flip;
}

/*
	getRestriction function
	returns a pointer to the SDL_Rect that bounds the object during the drawing process
*/
SDL_Rect *DrawObject::getRestriction(){
	return restrict;
}

/*
	getBlendMode function
	returns the BlendMode of the object
*/
BlendMode DrawObject::getBlendMode(){
	return blend;
}


/*
	setOffsetX function
	sets the x offset of the object
*/
bool DrawObject::setOffsetX(int o){
	offsetX = o;

	setX(getX());

	return offsetX == o;
}

/*
	setOffsetY function
	sets the y offset of the object
*/
bool DrawObject::setOffsetY(int o){
	offsetY = o;

	setY(getY());

	return offsetY == o;
}

/*
	setRotation function
	sets the angle of rotation of the object
	angle of rotation is represented in degrees
*/
bool DrawObject::setRotation(double r){
	rotation = r;

	return rotation == r;
}

/*setFlip function*/
bool DrawObject::setFlip(SDL_RendererFlip f){
	flip = f;

	return flip == f;
}

/*
	setRestriction function
	takes an X position, Y position, width, and height
	sets the draw boundries of the object based on those parameters
*/
bool DrawObject::setRestriction(int x, int y, uint width, uint height){
	if(restrict == NULL){
		restrict = new SDL_Rect;

		restrict->x = x;
		restrict->y = y;
		restrict->w = width;
		restrict->h = height;
	}else{
		restrict->x = x;
		restrict->y = y;
		restrict->w = width;
		restrict->h = height;
	}

	return restrict->x == x && restrict->y == y && restrict->w == width && restrict->h == height;
}

/*
	setRestriction function
	takes a pointer to a SDL_Rect
	sets the draw boundries of the object
*/
bool DrawObject::setRestriction(SDL_Rect *r){
	if(restrict == NULL){
		restrict = r;
	}else if(restrict != r){
		delete restrict;

		restrict = r;
	}

	return restrict == r;
}

/*
	setBlendMode function
	takes an pointer to a SDL_Rect
	sets the draw boundries of the object
*/
bool DrawObject::setBlendMode(BlendMode m){
	blend = m;
	SDL_SetTextureBlendMode(texture, (SDL_BlendMode)getBlendMode());

	return blend == m;
}


/*redraw the object*/
bool DrawObject::redrawObject(){
	redraw = true;

	return redraw;
}

bool DrawObject::copySection(Rectangle *src, Rectangle *dest, SDL_Renderer *renderer){
	if(texture != NULL){
		SDL_Rect srcRect = {(int)src->getX(), (int)src->getY(), src->getWidth(), src->getHeight()};
		SDL_Rect destRect = {(int)dest->getX(), (int)dest->getY(), dest->getWidth(), dest->getHeight()};

		SDL_RenderCopy(renderer, texture, &srcRect, &destRect);

		return true;
	}

	return false;
}


/*
	draw function
	takes an pointer to a SDL_Renderer
	draws the object's texture to the renderer
*/
bool DrawObject::draw(SDL_Renderer *renderer){
	if(texture == NULL || !textureLock || redraw || fullRedraw || lastRender < lastFullRedraw){
		SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, getTotalWidth(), getTotalHeight());

		if(tex){
			SDL_Texture *oldTarget = SDL_GetRenderTarget(renderer);
			SDL_SetRenderTarget(renderer, tex);

			SDL_SetTextureBlendMode(tex, (SDL_BlendMode)getBlendMode());
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);

			updateTexture(renderer);

			setTexture(tex);

			SDL_SetRenderTarget(renderer, oldTarget);
		}

		lastRender = SDL_GetTicks();
	}

	if(texture){
		SDL_Rect src;
		SDL_Rect dest;

		SDL_Point center = {0, 0};

		if(restrict){

			//if the texture is completely out of view on any side of the restriction
			if(getX() > restrict->x + restrict->w ||
					getX() + getWidth() < restrict->x ||
					getY() > restrict->y + restrict->h ||
					getY() + getHeight() < restrict->y){
				return false;
			}


			//if the left edge of the texture is outside of the restiction
			if(getX() < restrict->x){
				dest.x = restrict->x;
				src.x = restrict->x - getX();
			}else{
				dest.x = getX();
				src.x = 0;
			}


			//if the top edge of the texture is outside of the restiction
			if(getY() < restrict->y){
				dest.y = restrict->y;
				src.y = restrict->y - getY();
			}else{
				dest.y = getY();
				src.y = 0;
			}


			//if the right edge of the texture is outside of the restiction
			if(getX() + getWidth() > restrict->x + restrict->w){
				dest.w = restrict->x + restrict->w - dest.x;
			}else{
				dest.w = getX() + getWidth() - dest.x;
			}


			//if the bottom edge of the texture is outside of the restiction
			if(getY() + getHeight() > restrict->y + restrict->h){
				dest.h = restrict->y + restrict->h - dest.y;
			}else{
				dest.h = getY() + getHeight() - dest.y;
			}


			src.w = dest.w;
			src.h = dest.h;

			SDL_RenderCopyEx(renderer, texture, &src, &dest, rotation, NULL, flip);
		}else{
			int width = abs(getWidth() * cos(degreesToRadians(rotation))) + abs(getHeight() * sin(degreesToRadians(rotation)));
			int height = abs(getWidth() * sin(degreesToRadians(rotation))) + abs(getHeight() * cos(degreesToRadians(rotation)));

			dest = {(int)xPos + (width - getWidth())/2, (int)yPos + (height - getHeight())/2, getTotalWidth(), getTotalHeight()};

			SDL_RenderCopyEx(renderer, texture, NULL, &dest, rotation, NULL, flip);
		}


		if(redraw){
			redraw = false;
		}


		return true;
	}else{
		return false;
	}
}




/*
	getTexture function
	returns a pointer to the SDL_Texture of the object
*/
SDL_Texture *DrawObject::getTexture(){
	return texture;
}

/*
	setTexture function
	takes a pointer to a SDL_Texture
	sets the objects texture pointer to the parameter
*/
bool DrawObject::setTexture(SDL_Texture *tex){
	if(texture){
		SDL_DestroyTexture(texture);
	}

	texture = tex;

	return texture == tex;
}
