#ifndef RIBBON_H
#define RIBBON_H

#include <vector>
#include "Window.h"
#include "controls/RibbonOption.h"
#include "controls/Control.h"

class Ribbon : public Control{
	public:
		Ribbon(Window *win, std::vector<RibbonOption *> opt);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);

		Rectangle *base;
		std::vector<RibbonOption *> options;
		RibbonOption *selected;
		bool hoverMode;
};

#endif
