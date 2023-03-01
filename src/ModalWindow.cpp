#include <iostream>
#include "ModalWindow.h"


ModalWindow::ModalWindow(uint w, uint h, std::string title, Window *parent) : Window(w, h, title, false, parent){}

bool ModalWindow::blockingLoop(){
	while(isReady()){
		framePrep();

		frameCleanUp();
	}

	return true;
}
