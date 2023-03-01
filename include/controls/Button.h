#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <functional>

#include "objects/Rectangle.h"
#include "objects/Text.h"
#include "objects/Icon.h"
#include "controls/Control.h"


class Button : public Control{
	public:
		Button(int x, int y, std::string lab,
						std::function<bool()> onLeftClick=NULL,
						std::function<bool()> onRightClick=NULL,
						std::function<bool()> onMiddleClick=NULL);
		Button(int x, int y, Icon *ico,
						std::function<bool()> onLeftClick=NULL,
						std::function<bool()> onRightClick=NULL,
						std::function<bool()> onMiddleClick=NULL);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		std::string getLabel();
		Icon *getIcon();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
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

		bool leftClickAvailable();
		bool rightClickAvailable();
		bool middleClickAvailable();

		bool setHover(bool b);


		Rectangle *base;
		Text *text;
		Icon *icon;

		std::function<bool()> lClick;
		std::function<bool()> rClick;
		std::function<bool()> mClick;

		bool hasLeftClick;
		bool hasRightClick;
		bool hasMiddleClick;

		bool mouseDown;
		bool hover;
};

#endif
