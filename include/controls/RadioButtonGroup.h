#ifndef RADIOBUTTONGROUP_H
#define RADIOBUTTONGROUP_H

#include <string>
#include <vector>

#include "objects/Text.h"
#include "objects/Rectangle.h"
#include "controls/RadioButton.h"
#include "controls/Control.h"


class RadioButtonGroup : public Control{
	public:
		RadioButtonGroup(int x, int y, std::string lab, std::vector<std::string> b={});

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		std::string getLabel();
		std::string getSelection();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setLabel(std::string lab);
		bool setSelection(std::string s);

		bool addButton(std::string b);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		Rectangle *base;
		Text *label;

		std::vector<RadioButton *> buttons;
};

#endif
