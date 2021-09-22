#ifndef LABEL_H
#define LABEL_H

#include <string>

#include "objects/Text.h"
#include "controls/Control.h"


class Label : public Control{
	public:
		Label(int x, int y, uint s, std::string txt);

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
		bool setText(std::string s);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		Text *text;
};

#endif
