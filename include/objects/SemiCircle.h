#ifndef SEMICIRCLE_H
#define SEMICIRCLE_H

#include "Color.h"
#include "objects/DrawObject.h"

class SemiCircle : public DrawObject{
	public:
		SemiCircle(int x, int y, uint r, Color color=Color());

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		int getRadius();
		Color getFillColor();

		virtual bool setX(double x);
		virtual bool setY(double Y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setRadius(uint r);
		bool setFillColor(Color color);

		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual bool updateTexture(SDL_Renderer *renderer);


		uint radius;
		Color fillColor;
};

#endif
