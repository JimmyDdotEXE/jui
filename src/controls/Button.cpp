#include <iostream>
#include "controls/Button.h"


/*
	Button constructor
	takes an X position, Y position, and string
	constructs a label button
*/
Button::Button(int x, int y, std::string lab){
	text = new Text(1, 1, 12, lab);
	text->setPadding(-1*text->getDescent());

	base = new Rectangle(0, 0, text->getWidth()+2, text->getHeight()+2);
	base->outline(1, *accent);

	icon = NULL;

	active = false;

	hasLeftClick = false;
	hasRightClick = false;
	hasMiddleClick = false;

	setX(x);
	setY(y);

	//update theme to initialize all the colors correctly
	updateTheme();
}

/*
	Button constructor
	takes an X position, Y position, and pointer to an Icon
	constructs an icon button
*/
Button::Button(int x, int y, Icon *ico){
	icon = ico;
	icon->setX(1);
	icon->setY(1);

	base = new Rectangle(0, 0, icon->getWidth()+2, icon->getHeight()+2);
	base->outline(1, *accent);

	text = NULL;

	active = false;

	hasLeftClick = false;
	hasRightClick = false;
	hasMiddleClick = false;

	setX(x);
	setY(y);

	//update theme to initialize all the colors correctly
	updateTheme();
}


/*
	getWidth function
	returns the width of the button
*/
int Button::getWidth(){
	return base->getWidth();
}

/*
	getHeight function
	returns the height of the button
*/
int Button::getHeight(){
	return base->getHeight();
}

/*
	getTotalWidth function
	returns the total width of the button
*/
int Button::getTotalWidth(){
	return getWidth();
}

/*
	getTotalHeight function
	returns the total height of the button
*/
int Button::getTotalHeight(){
	return getHeight();
}

/*
	getActive function
	returns a boolean that tells if the button is active
*/
bool Button::getActive(){
	return active;
}

/*
	getLabel function
	returns the label of the button as a std::string
*/
std::string Button::getLabel(){
	if(text){
		return text->getString();
	}

	return "";
}

/*
	getIcon function
	returns a pointer to the Icon of the button
*/
Icon *Button::getIcon(){
	return icon;
}


/*
	setX function
	sets the X position of the button
*/
bool Button::setX(double x){
	base->setOffsetX(offsetX + x);

	if(text){
		text->setOffsetX(offsetX + x);
	}

	if(icon){
		icon->setOffsetX(offsetX + x);
	}

	xPos = x;

	return xPos == x;
}

/*
	setY function
	sets the Y position of the button
*/
bool Button::setY(double y){
	base->setOffsetY(offsetY + y);

	if(text){
		text->setOffsetY(offsetY + y);
	}

	if(icon){
		icon->setOffsetY(offsetY + y);
	}

	yPos = y;

	return yPos == y;
}

/*
	setWidth function
	sets the width of the button
	currently does nothing
*/
bool Button::setWidth(uint w){
	return false;
}

/*
	setHeight function
	sets the height of the button
	currently does nothing
*/
bool Button::setHeight(uint h){
	return false;
}

/*set whether the button is active or not*/
bool Button::setActive(bool b){
	active = b;

	if(b){
		textureLock = false;
	}else{
		textureLock = true;
		redraw = true;
	}

	return active == b;
}

/*set whether the button is focused on or not*/
bool Button::setFocus(bool b){

	if(b && !focus){
		base->setOutlineThickness(base->getOutlineThickness() * 2);
	}else if(!b && focus){
		base->setOutlineThickness(base->getOutlineThickness() / 2);
	}

	focus = b;
	redraw = true;

	return focus == b;
}

/*set the text of the button*/
bool Button::setLabel(std::string lab){
	if(text){
		text->setString(lab);
	}else if(icon){
		icon->free();
		delete icon;
		icon = NULL;

		text = new Text(1, 1, 12, lab);
		text->setPadding(-1*text->getDescent());

		base->setWidth(text->getWidth()+2);
	 	base->setHeight(text->getHeight()+2);

		updateTheme();
	}
	

	return text->getString() == lab;
}

/*set the icon of the button*/
bool Button::setIcon(Icon *ico){
	if(icon){
		icon = ico;
	}else if(text){
		text->free();
		delete text;
		text = NULL;

		icon = ico;
	}


	updateTheme();

	return icon == ico;
}


/*
	leftClick function
	function to excute if the button is left clicked
	does nothing until overridden in derived class
*/
bool Button::leftClick(){
	return false;
}

/*
	rightClick function
	function to excute if the button is right clicked
	does nothing until overridden in derived class
*/
bool Button::rightClick(){
	return false;
}

/*
	middleClick function
	function to excute if the button is middle clicked
	does nothing until overridden in derived class
*/
bool Button::middleClick(){
	return false;
}


/*
	handleEvent function
	takes a pointer to an Event
*/
bool Button::handleEvent(Event *event){

	//if the event is the start of a mouse click
	if(event->getType() == e_MOUSEPRESS && event->getMouseState() == s_DOWN){

		//if the mouse click happens within the bounds of the button
		if(boundsCheck(event->getX(), event->getY())){
			if(event->getButton() == b_LEFT && hasLeftClick ||
					event->getButton() == b_RIGHT && hasRightClick ||
					event->getButton() == b_MIDDLE && hasMiddleClick){
				setActive(true);

				event->setControl(this);
			}

			update();
			return active;
		}

	//if the event is the release of a mouse click while the button is active
	}else if(event->getType() == e_CLICK && active){

		setActive(false);
		update();

		if(boundsCheck(event->getX(), event->getY())){
			if(event->getButton() == b_LEFT && hasLeftClick){
				if(leftClick()){
					return true;
				}
			}else if(event->getButton() == b_RIGHT && hasRightClick){
				if(rightClick()){
					return true;
				}
			}else if(event->getButton() == b_MIDDLE && hasMiddleClick){
				if(middleClick()){
					return true;
				}
			}
		}

	//if the event is a mouse drag while the button is active
	}else if(event->getType() == e_DRAG && (active || focus)){

		if(active){
			setActive(false);
		}

		if(!boundsCheck(event->getX(), event->getY())){
			setFocus(false);
		}

		update();
		event->setControl(this);

		return false;


	//if the event is a mouse move
	}else if(event->getType() == e_MOVE){

		if(boundsCheck(event->getX(), event->getY()) && !boundsCheck(event->getPreviousX(), event->getPreviousY())){
			setFocus(true);
			return true;
		}else if(!boundsCheck(event->getX(), event->getY()) && boundsCheck(event->getPreviousX(), event->getPreviousY())){
			setFocus(false);
			return false;
		}	
	}

	return false;
}


/*updateTheme function*/
bool Button::updateTheme(){

	if(darkMode){
		base->setFillColor(*darkTheme[1]);

		if(active){
			if(text){
				text->setColor(base->getFillColor());
			}else if(icon){
				icon->setColor(base->getFillColor());
			}
		}else if(!active){
			if(text){
				text->setColor(*darkTheme[2]);
			}else if(icon){
				icon->setColor(*darkTheme[2]);
			}
		}

	}else{
		base->setFillColor(*lightTheme[1]);

		if(active){
			if(text){
				text->setColor(base->getFillColor());
			}else if(icon){
				icon->setColor(base->getFillColor());
			}
		}else if(!active){
			if(text){
				text->setColor(*lightTheme[2]);
			}else if(icon){
				icon->setColor(*lightTheme[2]);
			}	
		}

	}


	base->setOutlineColor(*accent);

	if(active){
		base->setFillColor(*accent);
	}

	redraw = true;
}

/*
	free function
	free and delete all the pieces of the button
*/
void Button::free(){
	base->free();

	if(text){
		text->free();
		delete text;
	}else if(icon){
		icon->free();
		delete icon;
	}

	delete base;

	setTexture(NULL);
}

/*
	boundsCheck function
	takes an X and Y position
	checks if the X and Y are within the bounds of the button
*/
bool Button::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update function
	called when an event is handled to update the visuals of the control
*/
void Button::update(){
	if(active){
		base->setFillColor(*accent);

		if(darkMode){
			if(text){
				text->setColor(*darkTheme[1]);
			}else if(icon){
				icon->setColor(*darkTheme[1]);
			}
		}else{
			if(text){
				text->setColor(*lightTheme[1]);
			}else if(icon){
				icon->setColor(*lightTheme[1]);
			}
		}

	}else{

		if(darkMode){
			base->setFillColor(*darkTheme[1]);

			if(text){
				text->setColor(*darkTheme[2]);
			}else if(icon){
				icon->setColor(*darkTheme[2]);
			}
		}else{
			base->setFillColor(*lightTheme[1]);

			if(text){
				text->setColor(*lightTheme[2]);
			}else if(icon){
				icon->setColor(*lightTheme[2]);
			}
		}
	}

	redraw = true;
}

/*update the texture of the button to be printed later*/
bool Button::updateTexture(SDL_Renderer *renderer){

		base->draw(renderer);

		if(text){
			text->draw(renderer);
		}else if(icon){
			icon->draw(renderer);
		}

}
