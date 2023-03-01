#include <iostream>
#include "controls/ComboBox.h"


/*
	ComboBox constructor

	int x: X position of the ComboBox
	int y: Y position of the ComboBox
	uint w: width of the ComboBox
	uint h: height of the ComboBox
*/
ComboBox::ComboBox(int x, int y, uint w, uint h){
	multipleEnabled = false;

	uint btnSize = h - 2;

	Icon *icon = new Icon(btnSize, btnSize);

	int offset = 0;
	for(int y=btnSize/4;y<btnSize/4 + btnSize/2;y+=2){

		for(int x=btnSize/4 + offset;x<btnSize/2;x++){
			icon->setPixel(x, y, Color(0, 0, 0, 255));
			icon->setPixel(btnSize-x-1, y, Color(0, 0, 0, 255));

			if(x==btnSize/4 + offset){
				icon->setPixel(x, y+1, Color(0, 0, 0, 255/2));
				icon->setPixel(btnSize-x-1, y+1, Color(0, 0, 0, 255/2));
			}else{
				icon->setPixel(x, y+1, Color(0, 0, 0, 255));
				icon->setPixel(btnSize-x-1, y+1, Color(0, 0, 0, 255));
			}
		}

		if(btnSize%2 == 1){
			icon->setPixel(btnSize/2, y, Color(0, 0, 0, 255));
			icon->setPixel(btnSize/2, y+1, Color(0, 0, 0, 255));

			if(y+2 >= btnSize/4 + btnSize/2){
				icon->setPixel(btnSize/2, y+2, Color(0, 0, 0, 255/2));
			}
		}

		offset++;
	}

	textBox = new TextBox(0, 0, w - h, h);

	button = new Button(textBox->getWidth(), 0, icon, [this](){
		return this->setDropDown(!this->getDropDown());
	});

	listView = new ListView(0, h, w, h * 10, h/2);

	dropDown = false;

	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int ComboBox::getWidth(){
	return textBox->getWidth() + button->getWidth();
}

/*get the height*/
int ComboBox::getHeight(){
	return textBox->getHeight();
}

/*get the total width*/
int ComboBox::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int ComboBox::getTotalHeight(){
	return textBox->getHeight() + listView->getHeight();
}

bool ComboBox::getDropDown(){
	return dropDown;
}

/*get the currently selected list elements*/
std::vector<std::string> ComboBox::getSelection(){
	return listView->getSelection();
}


/*set the X position*/
bool ComboBox::setX(double x){
	textBox->setOffsetX(offsetX + x);
	button->setOffsetX(offsetX + x);
	listView->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool ComboBox::setY(double y){
	textBox->setOffsetY(offsetY + y);
	button->setOffsetY(offsetY + y);
	listView->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function currently does nothing
*/
bool ComboBox::setWidth(uint w){
	return false;
}

/*
	set the height
	this function currently does nothing
*/
bool ComboBox::setHeight(uint h){
	return false;
}

/*set if the control is active*/
bool ComboBox::setDropDown(bool b){
	dropDown = b;

	if(b){
		textureLock = false;

		textBox->setText("");

		button->getIcon()->setRotation(180);
		button->redrawObject();
	}else{
		std::string s;
		std::vector<std::string> sel = listView->getSelection();

		for(int i=0;i<sel.size();i++){
			s += sel.at(i);

			if(i != sel.size()-1){
				s += ", ";
			}
		}

		textBox->setText(s);
		textBox->setActive(false);

		button->getIcon()->setRotation(0);
		button->redrawObject();

		setActiveControl(NULL);

		textureLock = true;
	}

	update();
	return dropDown == b;
}

/*set the list*/
bool ComboBox::setList(std::vector<std::string> s){
	listView->setList(s);
	update();
	return true;
}

/*set multiple current selections*/
bool ComboBox::setSelection(std::vector<std::string> s){
	listView->setSelection(s);
	setDropDown(false);
	return true;
}


/*try to handle the given event*/
bool ComboBox::handleEvent(Event *event){
	if(event->getControl() != NULL && event->getType() != e_MOVE && event->getType() != e_HOVER){
		setActive(false);
		setDropDown(false);
		update();
		return true;
	}

	bool ret = false;

	Control *aControl = getActiveControl();
	std::vector<std::string> sel = listView->getSelection();

	if(event->getType() == e_CLICK){
		if(!boundsCheck(event->getX(), event->getY()) && (getActive() || dropDown)){
			setActive(false);
			setDropDown(false);
			update();
			ret = true;
		}
	}


	if(aControl != NULL){
		aControl->handleEvent(event);

		if(event->getControl() == aControl){
			if(!aControl->getActive()){
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}
	}


	if(aControl != textBox){
		std::string text = textBox->getText();
		textBox->handleEvent(event);

		if(event->getControl() == textBox){
			if(textBox->getActive()){
				setActive(true);
				setActiveControl(textBox);

				if(!dropDown){
					setDropDown(true);
				}
			}else{
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}
	}


	if(aControl != button){
		button->handleEvent(event);

		if(event->getControl() == button){
			if(button->getActive()){
				setActive(true);
				setActiveControl(button);
			}else{
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}
	}


	if(dropDown && aControl != listView){
		listView->handleEvent(event);

		if(event->getControl() == listView){

			if(sel != listView->getSelection() && !multipleEnabled){
				setActive(false);
				setDropDown(false);
			}else if(listView->getActive()){
				setActive(true);
				setActiveControl(listView);
			}else{
				setActive(false);
				setDropDown(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}
	}

	return ret;
}


/*update the theme of the control*/
bool ComboBox::updateTheme(){
	textBox->updateTheme();
	button->updateTheme();
	listView->updateTheme();

	redraw = true;
	return true;
}

/*free all the memory used*/
void ComboBox::free(){
	textBox->free();
	button->free();
	listView->free();

	delete textBox;
	delete button;
	delete listView;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the ComboBox*/
bool ComboBox::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	if(!dropDown){
		return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
	}else{
		return x >= xPos && x < xPos + getTotalWidth() && y >= yPos && y < yPos + getTotalHeight();
	}
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void ComboBox::update(){
	listView->filter(textBox->getText());

	std::vector<t_Entry *> searchList = listView->getSearchList();

	if(searchList.size() == 0){
		listView->setHeight(0);
	}else if(searchList.size() < 10){
		listView->setHeight(searchList.size() * searchList.at(0)->text.getHeight() + 2);
	}else{
		listView->setHeight(10 * searchList.at(0)->text.getHeight() + 2);
	}

	redraw = true;
}

/*update the texture of the ComboBox to be printed later*/
bool ComboBox::updateTexture(SDL_Renderer *renderer){
	textBox->draw(renderer);
	button->draw(renderer);

	if(dropDown){
		listView->draw(renderer);
	}

	return true;
}
