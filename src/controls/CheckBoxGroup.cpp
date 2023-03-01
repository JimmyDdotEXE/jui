#include <algorithm>
#include "controls/CheckBoxGroup.h"


/*
	CheckBoxGroup constructor

	int x: X position of the CheckBoxGroup
	int y: Y position of the CheckBoxGroup
	std::string lab: label of the CheckBoxGroup
	std::vector<std::string> b: vector of names for CheckBoxes in the group
 */
CheckBoxGroup::CheckBoxGroup(int x, int y, std::string lab, std::vector<std::string> b){

	label = new Text(12, lab);
	label->setX(-1*label->getDescent() + 2);

	label->setFontSize(9);
	label->setLeftPadding(-1*label->getDescent());
	label->setRightPadding(-1*label->getDescent());
	label->setBlendMode(BLEND_NONE);


	base = new Rectangle(0, label->getAscent()/2, label->getX()*2 + label->getWidth(), label->getX()*2);
	base->setOutlineThickness(1);


	for(int i=0;i<b.size();i++){
		addBox(b.at(i));
	}

	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int CheckBoxGroup::getWidth(){
	return base->getWidth();
}

/*get the height*/
int CheckBoxGroup::getHeight(){
	return base->getY() + base->getHeight();
}

/*get the total width*/
int CheckBoxGroup::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int CheckBoxGroup::getTotalHeight(){
	return getHeight();
}

/*get the label of the CheckBoxGroup*/
std::string CheckBoxGroup::getLabel(){
	return label->getString();
}

/*get currently selected members of the CheckBoxGroup*/
std::vector<std::string> CheckBoxGroup::getSelection(){
	std::vector<std::string> ret;

	for(int i=0;i<boxes.size();i++){
		if(boxes.at(i)->getSelected()){
			ret.push_back(boxes.at(i)->getLabel());
		}
	}

	return ret;
}


/*set the X position*/
bool CheckBoxGroup::setX(double x){
	label->setOffsetX(offsetX + x);
	base->setOffsetX(offsetX + x);

	for(int i=0;i<boxes.size();i++){
		boxes.at(i)->setOffsetX(offsetX + x);
	}

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool CheckBoxGroup::setY(double y){
	label->setOffsetY(offsetY + y);
	base->setOffsetY(offsetY + y);

	for(int i=0;i<boxes.size();i++){
		boxes.at(i)->setOffsetY(offsetY + y);
	}

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool CheckBoxGroup::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool CheckBoxGroup::setHeight(uint h){
	return false;
}

/*set the label*/
bool CheckBoxGroup::setLabel(std::string lab){

	label->setString(lab);

	if(base->getWidth() - label->getX() - label->getWidth() < label->getX()){
		base->setWidth(label->getX()*2 + label->getWidth());
	}

	redraw = true;
	return true;
}

/*set the selected elements*/
bool CheckBoxGroup::setSelection(std::vector<std::string> s){
	bool ret = false;

	for(int i=0;i<boxes.size();i++){
		if(std::find(s.begin(), s.end(), boxes.at(i)->getLabel()) != s.end()){
			boxes.at(i)->setSelected(true);

			ret = true;
		}else if(boxes.at(i)->getSelected()){
			boxes.at(i)->setSelected(false);
		}
	}

	return ret;
}


/*add a new CheckBox with the given string to the group*/
bool CheckBoxGroup::addBox(std::string b){
	CheckBox *box = new CheckBox(0, 0, b);

	if(boxes.size() != 0){
		CheckBox *recent = boxes.at(boxes.size()-1);

		box->setX(recent->getX());
		box->setY(recent->getY() + recent->getHeight() + recent->getPadding());

	}else{
		box->setX(box->getPadding() + base->getOutlineThickness());
		box->setY(label->getY() + label->getHeight() + box->getPadding());
	}

	if(box->getX() + box->getWidth() + box->getPadding() + base->getOutlineThickness() > 
			base->getWidth()){

		base->setWidth(box->getX() + box->getWidth() + box->getPadding() + base->getOutlineThickness());
	}

	base->setHeight(box->getY() + box->getHeight() + box->getPadding() + base->getOutlineThickness() - base->getY());


	boxes.push_back(box);

	redraw = true;

	return true;
}


/*try to handle the given event*/
bool CheckBoxGroup::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	for(int i=0;i<boxes.size();i++){
		boxes.at(i)->handleEvent(event);

		if(event->getControl() == boxes.at(i)){

			event->setControl(this);
			redraw = true;
			return true;
		}
	}

	return false;
}


/*update the theme of the control*/
bool CheckBoxGroup::updateTheme(){

	if(darkMode){
		label->setColor(*darkTheme[2]);

		base->setFillColor(*darkTheme[0]);
		base->setOutlineColor(*darkTheme[2]);
	}else{
		label->setColor(*lightTheme[2]);

		base->setFillColor(*lightTheme[0]);
		base->setOutlineColor(*lightTheme[2]);
	}

	for(int i=0;i<boxes.size();i++){
		boxes.at(i)->updateTheme();
	}

	redraw = true;
	return true;
}

/*free all the memory used*/
void CheckBoxGroup::free(){
	label->free();
	delete label;

	for(int i=0;i<boxes.size();i++){
		boxes.at(i)->free();

		delete boxes.at(i);
	}

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the CheckBoxGroup*/
bool CheckBoxGroup::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void CheckBoxGroup::update(){
	redraw = true;
}

/*update the texture of the CheckBoxGroup to be printed later*/
bool CheckBoxGroup::updateTexture(SDL_Renderer *renderer){

	base->draw(renderer);
	label->draw(renderer);

	for(int i=0;i<boxes.size();i++){
		boxes.at(i)->draw(renderer);
	}

	return true;
}
