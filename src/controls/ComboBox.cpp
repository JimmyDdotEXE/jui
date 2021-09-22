#include <iostream>
#include "controls/ComboBox.h"


/*
	ComboBoxButton class
	derived from the button class
*/
class ComboBoxButton : public Button{
	public:
		ComboBoxButton(int x, int y, Icon *ico, ComboBox *combo) : Button(x, y, ico){
			comboBox = combo;

			hasLeftClick = true;
		}

		bool leftClick(){
			return comboBox->setActive(!comboBox->getActive());
		}

	protected:
		ComboBox *comboBox;
};


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

	button = new ComboBoxButton(textBox->getWidth(), 0, icon, this);

	listView = new ListView(0, h, w, h * 10, h/2);

	activeControl = NULL;

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

/*is the control active?*/
bool ComboBox::getActive(){
	return active;
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
bool ComboBox::setActive(bool b){
	active = b;

	if(b){
		textureLock = false;

		textBox->setText("");
		update();

		button->getIcon()->setRotation(180);
		button->redrawObject();

		if(focus){
			focus = false;
		}

	}else if(!focus){

		textBox->setText("");
		update();

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

		activeControl = NULL;

		textureLock = true;
		redraw = true;
	}

	return active == b;
}

/*set if the control is in focus*/
bool ComboBox::setFocus(bool b){
	focus = b;

	if(b){
		textureLock = false;

		if(active){
			setActive(false);
		}
	}else if(!active){
		textureLock = true;
		redraw = true;
	}

	return focus == b;
}

/*set the list*/
bool ComboBox::setList(std::vector<std::string> s){
	listView->setList(s);

	update();
}

/*set multiple current selections*/
bool ComboBox::setSelection(std::vector<std::string> s){
	listView->setSelection(s);
	setActive(false);
}


/*try to handle the given event*/
bool ComboBox::handleEvent(Event *event){

	std::vector<std::string> sel = listView->getSelection();

	if(event->getType() == e_MOVE || event->getType() == e_HOVER){
		if(boundsCheck(event->getX(), event->getY()) && !(active || focus)){
			setFocus(true);
		}else if(!boundsCheck(event->getX(), event->getY()) && focus){
			setFocus(false);
		}
	}else if(event->getType() == e_CLICK){
		if(!boundsCheck(event->getX(), event->getY()) && active){
			setActive(false);
		}
	}


	if(activeControl != NULL){
		activeControl->handleEvent(event);

		if(event->getControl() == activeControl){
			update();
			event->setControl(this);
			return true;
		}
	}


	if(activeControl != textBox){
		std::string text = textBox->getText();
		textBox->handleEvent(event);

		if(event->getControl() == textBox){

			if(!active){
				setActive(true);
			}

			if(text != textBox->getText()){
				update();
			}

			activeControl = textBox;
			event->setControl(this);
			return true;
		}
	}


	if(activeControl != button){
		button->handleEvent(event);

		if(event->getControl() == button){
			activeControl = button;
			event->setControl(this);
			return true;
		}
	}


	if(active && activeControl != listView){
		listView->handleEvent(event);

		if(event->getControl() == listView){

			if(sel != listView->getSelection() && !multipleEnabled){
				setActive(false);
			}

			activeControl = listView;
			event->setControl(this);
			return true;
		}
	}

	return false;
}


/*update the theme of the control*/
bool ComboBox::updateTheme(){
	textBox->updateTheme();
	button->updateTheme();
	listView->updateTheme();

	redraw = true;
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

	if(!active){
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
}

/*update the texture of the ComboBox to be printed later*/
bool ComboBox::updateTexture(SDL_Renderer *renderer){
	textBox->draw(renderer);
	button->draw(renderer);

	if(active){
		listView->draw(renderer);
	}
}
