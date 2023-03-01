#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <vector>
#include "controls/ContextMenuOption.h"
#include "controls/Control.h"

class ContextMenu : public Control{
	public:
		ContextMenu(std::vector<ContextMenuOption *> opt);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setHoverMode(bool *b);

		bool collapse();

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);

		Rectangle *base;
		std::vector<ContextMenuOption *> options;
		ContextMenuOption *selected;
		bool *hoverMode;
};

#endif
