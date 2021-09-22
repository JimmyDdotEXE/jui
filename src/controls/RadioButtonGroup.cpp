#include "controls/RadioButtonGroup.h"


/*
	RadioButtonGroup constructor

	int x: X position of the RadioButtonGroup
	int y: Y position of the RadioButtonGroup
	std::string lab: label of the RadioButtonGroup
	std::vector<std::string> b: vector of names for RadioButtons in the group
 */
RadioButtonGroup::RadioButtonGroup(int x, int y, std::string lab, std::vector<std::string> b){

	label = new Text(12, lab);
	label->setX(-1*label->getDescent() + 2);

	label->setFontSize(9);
	label->setLeftPadding(-1*label->getDescent());
	label->setRightPadding(-1*label->getDescent());
	label->setBlendMode(BLEND_NONE);


	base = new Rectangle(0, label->getAscent()/2, label->getX()*2 + label->getWidth(), label->getX()*2);
	base->setOutlineThickness(1);

	
	for(int i=0;i<b.size();i++){
		addButton(b.at(i));
	}

	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int RadioButtonGroup::getWidth(){
	return base->getWidth();
}

/*get the height*/
int RadioButtonGroup::getHeight(){
	return base->getY() + base->getHeight();
}

/*get the total width*/
int RadioButtonGroup::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int RadioButtonGroup::getTotalHeight(){
	return getHeight();
}

/*is the control active?*/
bool RadioButtonGroup::getActive(){
	return false;
}

/*get the label of the RadioButtonGroup*/
std::string RadioButtonGroup::getLabel(){
	return label->getString();
}

/*get currently selected member of the RadioButtonGroup*/
std::string RadioButtonGroup::getSelection(){
	for(int i=0;i<buttons.size();i++){
		if(buttons.at(i)->getActive()){
			return buttons.at(i)->getLabel();
		}
	}

	return "";
}


/*set the X position*/
bool RadioButtonGroup::setX(double x){
	label->setOffsetX(offsetX + x);
	base->setOffsetX(offsetX + x);

	for(int i=0;i<buttons.size();i++){
		buttons.at(i)->setOffsetX(offsetX + x);
	}

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool RadioButtonGroup::setY(double y){
	label->setOffsetY(offsetY + y);
	base->setOffsetY(offsetY + y);

	for(int i=0;i<buttons.size();i++){
		buttons.at(i)->setOffsetY(offsetY + y);
	}

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool RadioButtonGroup::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool RadioButtonGroup::setHeight(uint h){
	return false;
}

/*set the label*/
bool RadioButtonGroup::setLabel(std::string lab){

	label->setString(lab);

	if(base->getWidth() - label->getX() - label->getWidth() < label->getX()){
		base->setWidth(label->getX()*2 + label->getWidth());
	}

	redraw = true;
	return true;

}

/*set the selected element*/
bool RadioButtonGroup::setSelection(std::string s){
	bool ret = false;

	for(int i=0;i<buttons.size();i++){
		if(buttons.at(i)->getLabel() == s){
			buttons.at(i)->setActive(true);

			ret = true;
		}else if(buttons.at(i)->getActive()){
			buttons.at(i)->setActive(false);
		}
	}

	return ret;
}


/*add a new RadioButton with the given string to the group*/
bool RadioButtonGroup::addButton(std::string b){
	RadioButton *radio = new RadioButton(0, 0, b);

	if(buttons.size() != 0){
		RadioButton *recent = buttons.at(buttons.size()-1);

		radio->setX(recent->getX());
		radio->setY(recent->getY() + recent->getHeight() + recent->getPadding()/* + 1*/);

	}else{
		radio->setX(radio->getPadding() + base->getOutlineThickness());
		radio->setY(label->getY() + label->getHeight() + radio->getPadding());
	}

	if(radio->getX() + radio->getWidth() + radio->getPadding() + base->getOutlineThickness() > 
			base->getWidth()){

		base->setWidth(radio->getX() + radio->getWidth() + radio->getPadding() + base->getOutlineThickness());
	}

	base->setHeight(radio->getY() + radio->getHeight() + radio->getPadding() + base->getOutlineThickness() - base->getY());

	buttons.push_back(radio);

	redraw = true;

	return true;
}


/*try to handle the given event*/
bool RadioButtonGroup::handleEvent(Event *event){

	for(int i=0;i<buttons.size();i++){
		buttons.at(i)->handleEvent(event);

		if(event->getControl() == buttons.at(i)){

			for(int x=0;x<buttons.size();x++){
				if(buttons.at(x) != buttons.at(i)){
					buttons.at(x)->setActive(false);
				}
			}

			event->setControl(this);
			redraw = true;
			return true;
		}
	}

	return false;
}


/*update the theme of the control*/
bool RadioButtonGroup::updateTheme(){

	if(darkMode){
		label->setColor(*darkTheme[2]);

		base->setFillColor(*darkTheme[0]);
		base->setOutlineColor(*darkTheme[2]);

	}else{
		label->setColor(*lightTheme[2]);

		base->setFillColor(*lightTheme[0]);
		base->setOutlineColor(*lightTheme[2]);

	}

	for(int i=0;i<buttons.size();i++){
		buttons.at(i)->updateTheme();
	}

	redraw = true;
}

/*free all the memory used*/
void RadioButtonGroup::free(){
	label->free();
	delete label;

	for(int i=0;i<buttons.size();i++){
		buttons.at(i)->free();

		delete buttons.at(i);
	}

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the RadioButtonGroup*/
bool RadioButtonGroup::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
	this function does nothing
*/void RadioButtonGroup::update(){}

/*update the texture of the RadioButtonGroup to be printed later*/
bool RadioButtonGroup::updateTexture(SDL_Renderer *renderer){

	base->draw(renderer);
	label->draw(renderer);

	for(int i=0;i<buttons.size();i++){
		buttons.at(i)->draw(renderer);
	}

}
