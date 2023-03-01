#include "controls/Ribbon.h"


Ribbon::Ribbon(Window *win, std::vector<RibbonOption *> opt){
	options = opt;

	selected = NULL;
	hoverMode = false;

	base = new Rectangle(0, 0, win->getWidth(), 24);

	for(int i=0;i<options.size();i++){
		options.at(i)->setHoverMode(&hoverMode);

		if(i != 0){
			options.at(i)->setX(options.at(i-1)->getX()+options.at(i-1)->getWidth());
		}else{
			options.at(i)->setX(0);
			base->setHeight(options.at(i)->getHeight());
		}
	}

	setLeftLock(true);
	setRightLock(true);
	setLockResize(true);

	updateTheme();
	update();
}


int Ribbon::getWidth(){
	return base->getWidth();
}

int Ribbon::getHeight(){
	return base->getHeight();
}

int Ribbon::getTotalWidth(){
	return getWidth();
}

int Ribbon::getTotalHeight(){
	int h = getHeight();

	for(int i=0;i<options.size();i++){
		if(options.at(i)->getTotalHeight() > h){
			h = options.at(i)->getTotalHeight();
		}
	}

	return h;
}


bool Ribbon::setX(double x){
	return false;
}

bool Ribbon::setY(double y){
	return false;
}

bool Ribbon::setWidth(uint w){
	base->setWidth(w);
	update();	//TODO: this is here because edge lock resizing made things stretchy. should probably look into that
	return base->getWidth() == w;
}

bool Ribbon::setHeight(uint h){
	return false;
}


bool Ribbon::handleEvent(Event *event){
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

	if(event->getType() == e_MOUSEPRESS && event->getMouseState() == s_DOWN){
		if(base->boundsCheck(event->getX(), event->getY())){
			setActive(true);
			event->setControl(this);
			return true;
		}
	}

	return false;
}


bool Ribbon::updateTheme(){
	if(darkMode){
		base->setFillColor(*darkTheme[1]);
	}else{
		base->setFillColor(*lightTheme[1]);
	}

	for(int i=0;i<options.size();i++){
		options.at(i)->updateTheme();
	}

	redraw = true;
	return true;
}

void Ribbon::free(){
	base->free();
	delete base;
	base = NULL;

	for(int i=0;i<options.size();i++){
		options.at(i)->free();
		delete options.at(i);
	}

	setTexture(NULL);
}

bool Ribbon::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getTotalWidth() && y >= yPos && y < yPos + getTotalHeight();
}



void Ribbon::update(){
	if(!hoverMode && selected != NULL){
		selected->setSelected(false);
		selected = NULL;
	}

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

bool Ribbon::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);

	for(int i=0;i<options.size();i++){
		options.at(i)->draw(renderer);
	}

	return true;
}
