#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Color.h"
#include "objects/DrawObject.h"


class Rectangle : public DrawObject{
	public:
		Rectangle(int x, int y, uint w, uint h, Color color=Color());

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		int getOutlineThickness();
		Color getFillColor();
		Color getOutlineColor();

		virtual bool setX(double x);
		virtual bool setY(double Y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setOutlineThickness(uint thickness);
		bool setFillColor(Color color);
		bool setOutlineColor(Color color);

		bool outline(uint thickness, Color color=Color());

		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual bool updateTexture(SDL_Renderer *renderer);


		uint width;
		uint height;

		uint outlineThickness;

		Color fillColor;
		Color outlineColor;
};

#endif
