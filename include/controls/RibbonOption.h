#ifndef RIBBONOPTION_H
#define RIBBONOPTION_H

#include "controls/ContextMenu.h"
#include "controls/Button.h"

class Ribbon;

class RibbonOption : public Button{
	public:
		RibbonOption(std::string label, ContextMenu *menu=NULL);

		virtual int getTotalWidth();
		virtual int getTotalHeight();
		bool getSelected();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setSelected(bool b);
		bool setHoverMode(bool *b);

		virtual bool leftClick();

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);

		ContextMenu *subMenu;
		bool selected;
		bool *hoverMode;
};

#endif
