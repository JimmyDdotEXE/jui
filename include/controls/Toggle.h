#ifndef TOGGLE_H
#define TOGGLE_H

#include "objects/SemiCircle.h"
#include "objects/Circle.h"
#include "objects/Rectangle.h"
#include "controls/Control.h"


class Toggle : public Control{
	public:
		Toggle(int x, int y, bool *a=NULL);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		bool *getState();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setFocus(bool b);
		bool setState(bool *a);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		SemiCircle *baseLeft;
		SemiCircle *baseRight;
		Rectangle *baseMid;

		Circle *slider;

		bool *state;
};

#endif
