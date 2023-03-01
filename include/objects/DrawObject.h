#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include <SDL2/SDL.h>
#include "JUI.h"
#include "Color.h"

typedef enum{BLEND_NONE, BLEND_ALPHA} BlendMode;

class Rectangle;

class DrawObject{
	public:
		DrawObject();

		double getX();
		double getY();
		virtual int getWidth() =0;
		virtual int getHeight() =0;
		virtual int getTotalWidth() =0;
		virtual int getTotalHeight() =0;
		int getOffsetX();
		int getOffsetY();
		double getRotation();
		SDL_RendererFlip getFlip();
		SDL_Rect *getRestriction();
		BlendMode getBlendMode();

		virtual bool setX(double x) =0;
		virtual bool setY(double y) =0;
		virtual bool setWidth(uint w) =0;
		virtual bool setHeight(uint h) =0;
		bool setOffsetX(int o);
		bool setOffsetY(int o);
		bool setRotation(double r);
		bool setFlip(SDL_RendererFlip f);
		bool setRestriction(int x, int y, uint width, uint height);
		bool setRestriction(SDL_Rect *r);
		bool setBlendMode(BlendMode m);

		bool redrawObject();
		bool copySection(Rectangle *src, Rectangle *dest, SDL_Renderer *renderer);

		bool draw(SDL_Renderer *renderer);
		virtual void free() =0;
		virtual bool boundsCheck(int x, int y) =0;

	protected:
		SDL_Texture *getTexture();
		bool setTexture(SDL_Texture *tex);
		virtual bool updateTexture(SDL_Renderer *renderer) =0;


		double xPos;
		double yPos;

		int offsetX;
		int offsetY;

		double rotation;
		SDL_RendererFlip flip;
		SDL_Rect *restrict;
		SDL_Texture *texture;

		BlendMode blend;

		bool redraw;
		bool textureLock;
};

#endif
