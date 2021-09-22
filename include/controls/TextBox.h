#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>

#include "objects/Rectangle.h"
#include "objects/Text.h"
#include "objects/Line.h"
#include "objects/Icon.h"
#include "controls/Control.h"


class TextBox : public Control{
	public:
		TextBox(int x, int y, uint w=120, uint h=24, std::string txt="");

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		virtual bool getActive();
		std::string getText();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setActive(bool b);
		bool setFocus(bool b);
		bool setText(std::string s);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		Rectangle *base;
		Rectangle *highlightView;
		Text *leftText;
		Text *rightText;
		Text *highlightText;
		Text *label;
		Line *cursor;

		Icon *leftGradient;
		Icon *rightGradient;

		std::string fullString;
		uint cursorPos;
		bool mouseDown;
		int mouseX;

		uint startTime;
};

#endif
