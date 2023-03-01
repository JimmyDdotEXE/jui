#ifndef MODALWINDOW_H
#define MODALWINDOW_H

#include "Window.h"

class ModalWindow : public Window{
	public:
		ModalWindow(uint w, uint h, std::string title, Window *parent);

		bool blockingLoop();
};

#endif
