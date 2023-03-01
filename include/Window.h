#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Utility.h"
#include "Color.h"

#include "Event.h"
#include "objects/DrawObject.h"
#include "controls/Control.h"


class Window{
	public:
		Window(uint w, uint h, std::string title, bool resize=true, Window *parent=NULL);

		int getWidth();
		int getHeight();
		std::string getTitle();
		int getID();
		SDL_Window *getSDL_Window();
		Window *getParentWindow();
		Window *getModalWindow();

		void setWidth(uint w);
		void setHeight(uint h);
		bool setTitle(std::string s);
		bool setModalWindow(Window *modal);

		bool isReady();
		void close();

		bool mount();
		bool unmount();

		bool refreshRenderer();

		bool virtual handleEvent(Event *e);
		void handleControls(Event *e);

		void clear(Color color);
		void clear();
		bool postFrame();

		void draw(DrawObject *object);

		void addControl(Control *control);
		void removeControl(Control *control);
		bool drawControls();

		bool virtual checkWindowTheme();
		bool updateTheme();

	protected:
		SDL_Window *window;
		SDL_Renderer *renderer;

		int id;

		bool hidden;
		bool minimized;
		bool maximized;
		bool mouseFocus;
		bool keyboardFocus;

		Window *parentWindow;
		Window *modalWindow;

		std::vector<Control *> controls;
		Control *activeControl;
		uint width;
		uint height;

		std::string title;
		bool resizable;

		bool windowDark;
		bool windowInverted;
		Color windowAccent;

		Color clearColor;
};

#endif
