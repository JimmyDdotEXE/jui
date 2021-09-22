#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "General.h"
#include "Color.h"

#include "Event.h"
#include "objects/DrawObject.h"
#include "controls/Control.h"

extern int rad;


class Window{
	public:
		Window(uint w, uint h, std::string title, bool resize=true);

		int getWidth();
		int getHeight();
		std::string getTitle();

		void setWidth(uint w);
		void setHeight(uint h);
		bool setTitle(std::string s);

		bool isReady();
		void close();

		void handleEvents();
		void handleControls(Event *e);

		void clear(Color color=Color(0x00, 0x00, 0x00));
		void postFrame();

		void draw(DrawObject *object);

		void addControl(Control *control);
		void removeControl(Control *control);
		void drawControls();

		bool updateTheme();

	private:
		SDL_Window *window;
		SDL_Renderer *renderer;
		std::vector<Control *> controls;
		Control *activeControl;
		uint width;
		uint height;

		Event *event;

		bool windowDark;
		Color windowAccent;
};

#endif
