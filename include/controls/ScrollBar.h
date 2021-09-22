#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "objects/Line.h"
#include "objects/Rectangle.h"
#include "objects/SemiCircle.h"
#include "controls/Control.h"


class ScrollBar : public Control{
	public:
		ScrollBar(int x, int y, uint s, uint tS, uint iS, e_Orien orien=VERTICAL);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		virtual bool getActive();
		int getTotalSize();
		int getSliderSize();
		int getPos();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setActive(bool b);
		bool setTotalSize(uint s);
		bool setSliderSize(uint s);
		bool setPos(uint pos);

		bool scrollUp();
		bool scrollDown();

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		e_Orien orientation;

		uint totalSize;
		uint incrementSize;
		uint viewSize;
		uint sliderSize;

		int padding;

		Rectangle *base;
		Rectangle *sliderMid;
		SemiCircle *sliderTop;
		SemiCircle *sliderBottom;

		int grab;
};

#endif
