#ifndef SLIDER_H
#define SLIDER_H

#include "objects/Rectangle.h"
#include "objects/Circle.h"
#include "objects/SemiCircle.h"
#include "objects/Line.h"
#include "controls/Control.h"


class Slider : public Control{
	public:
		Slider(int x, int y, uint size, double mi, double ma, double *n, e_Orien orien=HORIZONTAL);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		double getMin();
		double getMax();
		double *getTarget();
		double getValue();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setMin(double m);
		bool setMax(double m);
		bool setTarget(double *t);
		bool setValue(double v);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);


	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		double incrementSize;

		double min;
		double max;
		double *target;

		SemiCircle *edgeLeft;
		SemiCircle *edgeRight;
		Rectangle *baseLeft;
		Rectangle *baseRight;
		Circle *sliderBase;
		Circle *sliderMid;

		e_Orien orientation;

		int grab;
};

#endif
