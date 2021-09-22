#include <iostream>
#include <algorithm>
#include <cmath>
#include "controls/ListView.h"


/*
	ListView constructor

	int x: X position of the ListView
	int y: Y position of the ListView
	uint w: width of the ListView
	uint h: height of the ListView
	uint size: font size in the ListView
 */
ListView::ListView(int x, int y, uint w, uint h, uint size){
	multipleEnabled = false;
	textureLock = false;

	fontSize = size;

	base = new Rectangle(0, 0, 1, 1);
	base->outline(1);
	view = new View(1, 1, 1, 1);

	setWidth(w);
	setHeight(h);

	setX(x);
	setY(y);

	updateTheme();
}

/*
	ListView constructor

	int x: X position of the ListView
	int y: Y position of the ListView
	uint w: width of the ListView
	uint h: height of the ListView
	std::vector<std::string> ls: vector of strings to be in the ListView
	uint size: font size in the ListView
 */
ListView::ListView(int x, int y, uint w, uint h, std::vector<std::string> ls, uint size){
	multipleEnabled = false;
	textureLock = false;

	fontSize = size;

	base = new Rectangle(0, 0, 1, 1);
	base->outline(1);
	view = new View(1, 1, 1, 1);

	setWidth(w);
	setHeight(h);

	setList(ls);

	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int ListView::getWidth(){
	return base->getWidth();
}

/*get the height*/
int ListView::getHeight(){
	return base->getHeight();
}

/*get the total width*/
int ListView::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int ListView::getTotalHeight(){
	return getHeight();
}

/*
	is the control active?
	the function always returns false
*/
bool ListView::getActive(){
	return false;
}

/*get the current search results of the list*/
std::vector<t_Entry *> ListView::getSearchList(){
	return searchList;
}

/*get the currently selected list elements*/
std::vector<std::string> ListView::getSelection(){
	std::vector<std::string> ret;

	for(int i=0;i<selected.size();i++){
		ret.push_back(selected.at(i)->text.getString());
	}

	return ret;
}


/*set the X position*/
bool ListView::setX(double x){
	base->setOffsetX(offsetX + x);
	view->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool ListView::setY(double y){
	base->setOffsetY(offsetY + y);
	view->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*set the width*/
bool ListView::setWidth(uint w){
	base->setWidth(w);

	view->setWidth(w - base->getOutlineThickness()*2);

	return base->getWidth() == w;
}

/*set the height*/
bool ListView::setHeight(uint h){
	base->setHeight(h);

	view->setHeight(h - base->getOutlineThickness()*2);

	return base->getHeight() == h;
}

/*set the font size of the text elements*/
bool ListView::setFontSize(uint s){
	fontSize = s;

	for(int i=0;i<fullList.size();i++){
		fullList.at(i).text.setFontSize(s);
		fullList.at(i).text.setPadding(-1*fullList.at(i).text.getDescent());

		if(i != 0){
			fullList.at(i).text.setY(fullList.at(i).text.getY() + fullList.at(i).text.getHeight());
			fullList.at(i).highlight.setY(fullList.at(i).text.getY());
		}

		fullList.at(i).highlight.setHeight(fullList.at(i).text.getHeight());
	}

	return fontSize == s;
}

/*set the list*/
bool ListView::setList(std::vector<std::string> s){

	selected.clear();
	searchList.clear();
	fullList.clear();

	for(int i=0;i<s.size();i++){

		Text t = Text(fontSize, s.at(i));
		t.setBlendMode(BLEND_NONE);
		t.setPadding(-1*t.getDescent());
		t.setX(0);

		if(i == 0){
			t.setY(0);
		}else{
			t.setY(fullList.at(i-1).text.getY() + fullList.at(i-1).text.getHeight());
		}


		Rectangle r = Rectangle(t.getX(), t.getY(), view->getWidth(), t.getHeight());

		fullList.push_back({t, r});

	}

	filter("");
	updateTheme();

	return true;
}

/*set the current selection*/
bool ListView::setSelection(std::string s){

	for(int i=0;i<fullList.size();i++){

		if(fullList.at(i).text.getString() == s){
			selected.clear();

			selected.push_back(&fullList.at(i));

			return true;
		}

	}

	update();

	return false;
}

/*set multiple current selections*/
bool ListView::setSelection(std::vector<std::string> s){
	selected.clear();

	for(int i=0;i<fullList.size();i++){

		if(std::find(s.begin(), s.end(), fullList.at(i).text.getString()) != s.end()){
			selected.push_back(&fullList.at(i));
		}

	}

	update();

	return true;
}


/*filter the list*/
bool ListView::filter(std::string s, e_Search type){
	searchList.clear();

	if(s == ""){
		for(int i=0;i<fullList.size();i++){
			searchList.push_back(&fullList.at(i));
		}
	}else{

		for(int i=0;i<fullList.size();i++){
			if(type == GENERAL){
				if(fullList.at(i).text.getString().find(s) != std::string::npos){
					searchList.push_back(&fullList.at(i));
				}
			}else if(type == FIRST){
				if(s == fullList.at(i).text.getString().substr(0, s.length())){
					searchList.push_back(&fullList.at(i));
				}
			}
		}
	}

	for(int i=0;i<searchList.size();i++){
		searchList.at(i)->text.setY(i * searchList.at(i)->text.getHeight());
		searchList.at(i)->highlight.setY(searchList.at(i)->text.getY());
	}

	view->clear();
}



/*try to handle the given event*/
bool ListView::handleEvent(Event *event){

	view->handleEvent(event);

	if(event->getControl() == view){

		event->setControl(this);
		redraw = true;

	}else if(event->getType() == e_CLICK && boundsCheck(event->getX(), event->getY())){

		if(event->getButton() == b_LEFT){

			for(int i=0;i<searchList.size();i++){
				
				if(searchList.at(i)->highlight.boundsCheck(event->getX(), event->getY())){

					if(multipleEnabled){

					}else{
						if(std::find(selected.begin(), selected.end(), searchList.at(i)) == selected.end()){
							selected.clear();

							selected.push_back(searchList.at(i));
							update();

							event->setControl(this);
						}

					}

				}

			}

		}

	}
}


/*update the theme of the control*/
bool ListView::updateTheme(){

	view->updateTheme();

	if(darkMode){
		base->setFillColor(*darkTheme[1]);
		base->setOutlineColor(*darkTheme[3]);

		for(int i=0;i<fullList.size();i++){
			fullList.at(i).text.setColor(*darkTheme[2]);
			fullList.at(i).highlight.setFillColor(Color());
		}

		for(int i=0;i<selected.size();i++){
			selected.at(i)->text.setColor(base->getFillColor());
			selected.at(i)->highlight.setFillColor(*accent);
		}
	}else{
		base->setFillColor(*lightTheme[1]);
		base->setOutlineColor(*lightTheme[3]);

		for(int i=0;i<fullList.size();i++){
			fullList.at(i).text.setColor(*lightTheme[2]);
			fullList.at(i).highlight.setFillColor(Color());
		}

		for(int i=0;i<selected.size();i++){
			selected.at(i)->text.setColor(base->getFillColor());
			selected.at(i)->highlight.setFillColor(*accent);
		}
	}

	redraw = true;
}

/*free all the memory used*/
void ListView::free(){
	base->free();
	view->free();

	delete base;
	delete view;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the ListView*/
bool ListView::boundsCheck(int x, int y){
	x -= offsetX;
	y -= offsetY;

	return x >= xPos && x < xPos + base->getWidth() && y >= yPos && y < yPos + base->getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void ListView::update(){

	for(int i=0;i<fullList.size();i++){
		if(fullList.at(i).highlight.getFillColor().getUint() != 0x00000000){
			fullList.at(i).highlight.setFillColor(Color());
		}

		if(darkMode){
			if(fullList.at(i).text.getColor().getUint() != darkTheme[2]->getUint()){
				fullList.at(i).text.setColor(*darkTheme[2]);
				fullList.at(i).text.setBlendMode(BLEND_NONE);
			}
		}else{
			if(fullList.at(i).text.getColor().getUint() != lightTheme[2]->getUint()){
				fullList.at(i).text.setColor(*lightTheme[2]);
				fullList.at(i).text.setBlendMode(BLEND_NONE);
			}
		}
	}

	for(int i=0;i<selected.size();i++){
		if(selected.at(i)->highlight.getFillColor().getUint() != accent->getUint()){
			selected.at(i)->highlight.setFillColor(*accent);
		}

		if(selected.at(i)->text.getColor().getUint() != base->getFillColor().getUint()){
			selected.at(i)->text.setColor(base->getFillColor());
			selected.at(i)->text.setBlendMode(BLEND_ALPHA);
		}
	}

	redraw = true;
}

/*update the texture of the ListView to be printed later*/
bool ListView::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);

	for(int i=0;i<searchList.size();i++){
		view->drawObject(&searchList.at(i)->highlight);
		view->drawObject(&searchList.at(i)->text);
	}

	view->draw(renderer);
}
