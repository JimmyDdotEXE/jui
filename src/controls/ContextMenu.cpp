#include "controls/ContextMenu.h"


ContextMenu::ContextMenu(std::vector<ContextMenuOption *> opt){
	options = opt;

	selected = NULL;
	hoverMode = NULL;

	base = new Rectangle(0, 0, 0, 0);

	int width = 0;
	for(int i=0;i<options.size();i++){
		options.at(i)->setX(0);
		options.at(i)->setY(options.at(i)->getHeight()*i);

		base->setHeight(options.at(i)->getY() + options.at(i)->getHeight());

		if(options.at(i)->getWidth() > width){
			width = options.at(i)->getWidth();
		}
	}

	setWidth(width);

	update();
}


int ContextMenu::getWidth(){
	return base->getWidth();
}

int ContextMenu::getHeight(){
	return base->getHeight();
}

int ContextMenu::getTotalWidth(){
	int width = getWidth();

	for(int i=0;i<options.size();i++){
		if(options.at(i)->getTotalWidth() > width){
			width = options.at(i)->getTotalWidth();
		}
	}

	return width;
}

int ContextMenu::getTotalHeight(){
	int height = getHeight();

	for(int i=0;i<options.size();i++){
		if(options.at(i)->getTotalHeight() + options.at(i)->getY() > height){
			height = options.at(i)->getTotalHeight() + options.at(i)->getY();
		}
	}

	return height;
}


bool ContextMenu::setX(double x){
	base->setOffsetX(offsetX + x);

	for(int i=0;i<options.size();i++){
		options.at(i)->setOffsetX(offsetX + x);
	}

	xPos = x;

	return xPos == x;
}

bool ContextMenu::setY(double y){
	base->setOffsetY(offsetY + y);

	for(int i=0;i<options.size();i++){
		options.at(i)->setOffsetY(offsetY + y);
	}

	yPos = y;

	return yPos == y;
}

bool ContextMenu::setWidth(uint w){
	for(int i=0;i<options.size();i++){
		options.at(i)->setWidth(w);
	}
	
	base->setWidth(w);

	return base->getWidth() == w;
}

bool ContextMenu::setHeight(uint h){
	return false;
}

bool ContextMenu::setHoverMode(bool *b){
	for(int i=0;i<options.size();i++){
		options.at(i)->setHoverMode(b);
	}

	hoverMode = b;
	return hoverMode == b;
}


bool ContextMenu::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	for(int i=0;i<options.size();i++){
		options.at(i)->handleEvent(event);

		if(event->getControl() == options.at(i)){
			event->setControl(this);
			update();
			return true;
		}
	}

	return false;
}


bool ContextMenu::collapse(){
	if(selected != NULL){
		selected->setSelected(false);
		selected = NULL;
	}

	return true;
}


bool ContextMenu::updateTheme(){
	for(int i=0;i<options.size();i++){
		options.at(i)->updateTheme();
	}

	redraw = true;
	return true;
}

void ContextMenu::free(){
	for(int i=0;i<options.size();i++){
		options.at(i)->free();
		delete options.at(i);
	}

	options.clear();

	base->free();
	delete base;
	base = NULL;

	setTexture(NULL);
}

bool ContextMenu::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




void ContextMenu::update(){
	for(int i=0;i<options.size();i++){
		if(options.at(i)->getSelected() && options.at(i) != selected){
			if(selected != NULL){
				selected->setSelected(false);
			}

			selected = options.at(i);
		}
	}

	redraw = true;
}

bool ContextMenu::updateTexture(SDL_Renderer *renderer){
	for(int i=0;i<options.size();i++){
		options.at(i)->draw(renderer);
	}

	return true;
}
