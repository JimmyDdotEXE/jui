#ifndef CONTEXTMENUOPTION_H
#define CONTEXTMENUOPTION_H

#include "controls/Button.h"

class ContextMenu;
class Window;

class ContextMenuOption : public Button{
	public:
		ContextMenuOption(std::string label, std::function<bool()> onLeftClick, std::function<bool()> available=NULL);
		ContextMenuOption(std::string label, ContextMenu *menu, std::function<bool()> available=NULL);

		virtual int getTotalWidth();
		virtual int getTotalHeight();
		bool getSelected();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setSelected(bool b);
		bool setHoverMode(bool *b);

		bool checkAvailability();

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);

		ContextMenu *subMenu;
		Icon *arrow;
		bool selected;
		bool *hoverMode;
		std::function<bool()> availabilityCheck;
};

#endif
