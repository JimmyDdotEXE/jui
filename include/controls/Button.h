#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "objects/Rectangle.h"
#include "objects/Text.h"
#include "objects/Icon.h"
#include "controls/Control.h"


class Button : public Control{
	public:
		Button(int x, int y, std::string lab);
		Button(int x, int y, Icon *ico);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		virtual bool getActive();
		std::string getLabel();
		Icon *getIcon();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setActive(bool b);
		bool setFocus(bool b);
		bool setLabel(std::string lab);
		bool setIcon(Icon *ico);

		virtual bool leftClick();
		virtual bool rightClick();
		virtual bool middleClick();

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		Rectangle *base;
		Text *text;
		Icon *icon;

		bool hasLeftClick;
		bool hasRightClick;
		bool hasMiddleClick;
};

#endif
