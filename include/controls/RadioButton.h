#ifndef RADIOBUTTON_H
#define RADIOBUTTON_H

#include <string>

#include "objects/Text.h"
#include "objects/Circle.h"
#include "Control.h"


class RadioButton : public Control{
	public:
		RadioButton(int x, int y, std::string lab);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		virtual bool getActive();
		std::string getLabel();
		int getPadding();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setActive(bool a);
		bool setLabel(std::string lab);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		Circle *base;
		Circle *trim;
		Circle *center;
		Text *text;
};

#endif
