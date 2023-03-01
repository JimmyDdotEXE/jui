#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <string>

#include "objects/Text.h"
#include "objects/Rectangle.h"
#include "controls/Control.h"


class CheckBox : public Control{
	public:
		CheckBox(int x, int y, std::string lab);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		bool getSelected();
		std::string getLabel();
		int getPadding();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setSelected(bool b);
		bool setLabel(std::string lab);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);

		bool selected;

		Rectangle *base;
		Rectangle *center;
		Text *text;
};

#endif
