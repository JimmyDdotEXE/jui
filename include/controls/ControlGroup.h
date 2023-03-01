#ifndef CONTROLGROUP_H
#define CONTROLGROUP_H

#include <vector>
#include <string>

#include "objects/Rectangle.h"
#include "controls/Control.h"


class ControlGroup : public Control{
	public:
		ControlGroup(int x, int y, uint w, uint h, std::vector<Control *> c={});

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		bool getTextureLock();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setTextureLock(bool b);

		bool addControl(Control *control);
		bool removeControl(Control *control);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		Rectangle *base;

		std::vector<Control *> controls;
};

#endif
