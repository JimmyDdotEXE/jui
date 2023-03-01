#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <string>

#include "controls/TextBox.h"
#include "controls/Button.h"
#include "controls/ListView.h"
#include "controls/Control.h"


class ComboBox : public Control{
	public:
		ComboBox(int x, int y, uint w=96, uint h=24);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		bool getDropDown();
		std::vector<std::string> getSelection();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setDropDown(bool b);
		bool setList(std::vector<std::string> s);
		bool setSelection(std::vector<std::string> s);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		bool multipleEnabled;
		bool dropDown;

		TextBox *textBox;
		Button *button;
		ListView *listView;
};

#endif
