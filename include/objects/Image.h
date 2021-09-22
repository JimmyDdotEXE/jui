#ifndef IMAGE_H
#define IMAGE_H

#include "Color.h"
#include "objects/Rectangle.h"
#include "objects/DrawObject.h"

#include <SDL2/SDL.h>
#include <string>


class Image : public DrawObject{
	public:
		Image(int x, int y, std::string fileName);
		Image(int x, int y, uint w, uint h, Color color=Color());

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		Color getPixel(uint x, uint y);
		int getScale();

		virtual bool setX(double x);
		virtual bool setY(double Y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setPixel(uint x, uint y, Color color);
		bool setScale(uint s);

		bool resize(uint w, uint h, int xOff, int yOff, uint fillColor);

		bool load(std::string fileName);

		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual bool updateTexture(SDL_Renderer *renderer);


		SDL_Surface *image;
		std::string file;
		uint width;
		uint height;
		uint scale;
};

#endif
