#ifndef LINE_H
#define LINE_H

#include "Color.h"
#include "objects/DrawObject.h"

typedef enum{HORIZONTAL, VERTICAL} e_Orien;

class Line : public DrawObject{
	public:
		Line(int x, int y, uint len, Color colo, e_Orien orien);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		int getLength();
		Color getColor();

		virtual bool setX(double x);
		virtual bool setY(double Y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setLength(uint len);
		bool setColor(Color colo);

		void rotate();

		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual bool updateTexture(SDL_Renderer *renderer);


		e_Orien orientation;
		uint length;
		Color color;
};

#endif
