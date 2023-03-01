#include <iostream>

#include "controls/ScrollBar.h"


/*
	ScrollBar constructor

	int x: X position of the ScrollBar
	int y: Y position of the ScrollBar
	uint s: size of the ScrollBar
	uint tS: total size of the content being scrolled
	uint iS: increment size for mouse wheel scrolls in pixels
	e_Orien orien: orientation of the ScrollBar
 */
ScrollBar::ScrollBar(int x, int y, uint s, uint tS, uint iS, e_Orien orien){
	totalSize = tS;
	incrementSize = iS;

	
	orientation = orien;


	if(orientation == VERTICAL){
		base = new Rectangle(0, 0, 16, s);

		padding = getWidth() * .125;

		sliderSize = (getHeight() - padding*2) * getHeight() / totalSize;

		sliderTop = new SemiCircle(padding, padding, (getWidth() / 2) - padding);
		sliderTop->setRotation(180);
		sliderMid = new Rectangle(padding, sliderTop->getY() + sliderTop->getRadius(),
															getWidth() - padding*2, sliderSize - sliderTop->getRadius()*2);
		sliderBottom = new SemiCircle(padding, sliderMid->getY() + sliderMid->getHeight(), sliderTop->getRadius());
	}else{
		base = new Rectangle(0, 0, s, 16);

		padding = getHeight() * .125;

		sliderSize = (getWidth() - padding*2) * getWidth() / totalSize;

		sliderTop = new SemiCircle(padding, padding, (getHeight() / 2) - padding);
		sliderTop->setRotation(90);
		sliderMid = new Rectangle(sliderTop->getX() + sliderTop->getRadius(), padding,
															sliderSize - sliderTop->getRadius()*2, getHeight() - padding*2);
		sliderBottom = new SemiCircle(sliderMid->getX() + sliderMid->getWidth(), padding, sliderTop->getRadius());
		sliderBottom->setRotation(270);
	}


	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int ScrollBar::getWidth(){
	return base->getWidth();
}

/*get the height*/
int ScrollBar::getHeight(){
	return base->getHeight();
}

/*get the total width*/
int ScrollBar::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int ScrollBar::getTotalHeight(){
	return getHeight();
}

/*get the size of the content being scrolled*/
int ScrollBar::getTotalSize(){
	return totalSize;
}

/*get the size of the slider*/
int ScrollBar::getSliderSize(){
	return sliderSize;
}

/*get the position of the slider in the ScrollBar*/
int ScrollBar::getPos(){
	if(orientation == VERTICAL){
		return totalSize * (sliderTop->getY()-padding) / (getHeight()-padding*2);
	}else{
		return totalSize * (sliderTop->getX()-padding) / (getWidth()-padding*2);
	}
}


/*set the X position*/
bool ScrollBar::setX(double x){

	base->setOffsetX(offsetX + x);
	sliderTop->setOffsetX(offsetX + x);
	sliderMid->setOffsetX(offsetX + x);
	sliderBottom->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool ScrollBar::setY(double y){

	base->setOffsetY(offsetY + y);
	sliderTop->setOffsetY(offsetY + y);
	sliderMid->setOffsetY(offsetY + y);
	sliderBottom->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*
	set the width
	fails if the ScrollBar is vertical
*/
bool ScrollBar::setWidth(uint w){

	if(orientation == HORIZONTAL){
		base->setWidth(w);

		setSliderSize((w - padding*2) * w / totalSize);

		redraw = true;

		return getWidth() == w;
	}else{
		return false;
	}
}
/*
	set the height
	fails if the ScrollBar is horizontal
*/
bool ScrollBar::setHeight(uint h){

	if(orientation == VERTICAL){
		base->setHeight(h);

		setSliderSize((h - padding*2) * h / totalSize);

		redraw = true;

		return getHeight() == h;
	}else{
		return false;
	}
}

/*tell the ScrollBar how big the content it's scrolling is*/
bool ScrollBar::setTotalSize(uint s){

	totalSize = s;

	if(orientation == VERTICAL){
		setSliderSize((getHeight() - padding*2) * getHeight() / s);
	}else{
		setSliderSize((getWidth() - padding*2) * getWidth() / s);
	}

	redraw = true;

	return true;
}

/*set the size of the slider*/
bool ScrollBar::setSliderSize(uint s){
	sliderSize = s;

	if(orientation == VERTICAL){
		sliderMid->setHeight(sliderSize - sliderTop->getRadius()*2);
		sliderBottom->setY(sliderMid->getY() + sliderMid->getHeight());
	}else{
		sliderMid->setWidth(sliderSize - sliderTop->getRadius()*2);
		sliderBottom->setX(sliderMid->getX() + sliderMid->getWidth());
	}

	redraw = true;

	return true;
}

/*set the position of the slider in the ScrollBar*/
bool ScrollBar::setPos(uint pos){

	if(orientation == VERTICAL){
		sliderTop->setY((getHeight()- padding*2) * pos / totalSize + padding);

		if(sliderTop->getY() + sliderSize > getHeight() - padding){
			sliderTop->setY(getHeight() - padding - sliderSize);
		}

		sliderMid->setY(sliderTop->getY() + sliderTop->getRadius());
		sliderBottom->setY(sliderMid->getY() + sliderMid->getHeight());
	}else{
		sliderTop->setX((getWidth()- padding*2) * pos / totalSize + padding);

		if(sliderTop->getX() + sliderSize > getWidth() - padding){
			sliderTop->setX(getWidth() - padding - sliderSize);
		}

		sliderMid->setX(sliderTop->getX() + sliderTop->getRadius());
		sliderBottom->setX(sliderMid->getX() + sliderMid->getWidth());
	}


	redraw = true;

	return true;
}


/*scroll the content up/left*/
bool ScrollBar::scrollUp(){

	if(orientation == VERTICAL){
		int scrollAmount = (getHeight() - padding*2) * incrementSize/totalSize;

		sliderTop->setY(sliderTop->getY() - scrollAmount);

		if(sliderTop->getY() < padding){
			sliderTop->setY(padding);
		}

		sliderMid->setY(sliderTop->getY() + sliderTop->getRadius());
		sliderBottom->setY(sliderMid->getY() + sliderMid->getHeight());
	}else{
		int scrollAmount = (getWidth() - padding*2) * incrementSize/totalSize;

		sliderTop->setX(sliderTop->getX() - scrollAmount);

		if(sliderTop->getX() < padding){
			sliderTop->setX(padding);
		}

		sliderMid->setX(sliderTop->getX() + sliderTop->getRadius());
		sliderBottom->setX(sliderMid->getX() + sliderMid->getWidth());
	}

	redraw = true;
	return true;
}

/*scroll the content down/right*/
bool ScrollBar::scrollDown(){

	if(orientation == VERTICAL){
		int scrollAmount = (getHeight() - padding*2) * incrementSize/totalSize;

		sliderTop->setY(sliderTop->getY() + scrollAmount);

		if(sliderTop->getY() + sliderSize > getHeight() - padding){
			sliderTop->setY(getHeight() - padding - sliderSize);
		}

		sliderMid->setY(sliderTop->getY() + sliderTop->getRadius());
		sliderBottom->setY(sliderMid->getY() + sliderMid->getHeight());
	}else{
		int scrollAmount = (getWidth() - padding*2) * incrementSize/totalSize;

		sliderTop->setX(sliderTop->getX() + scrollAmount);

		if(sliderTop->getX() + sliderSize > getWidth() - padding){
			sliderTop->setX(getWidth() - padding - sliderSize);
		}

		sliderMid->setX(sliderTop->getX() + sliderTop->getRadius());
		sliderBottom->setX(sliderMid->getX() + sliderMid->getWidth());
	}

	redraw = true;
	return true;
}


/*try to handle the given event*/
bool ScrollBar::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	//Handle scroll wheel event
	if(event->getType() == e_SCROLL){

		if(boundsCheck(event->getX(), event->getY())){

			if(event->getScrollDirection() == w_UP){
				scrollUp();
				update();
				event->setControl(this);
				return true;
			}else if(event->getScrollDirection() == w_DOWN){
				scrollDown();
				update();
				event->setControl(this);
				return true;
			}
		}

	//Handle mouse pressed event
	}else if(event->getType() == e_MOUSEPRESS && event->getMouseState() == s_DOWN){

		if(event->getButton() == b_LEFT){

			if(sliderTop->boundsCheck(event->getX(), event->getY()) ||
					sliderMid->boundsCheck(event->getX(), event->getY()) ||
					sliderBottom->boundsCheck(event->getX(), event->getY())){

				setActive(true);

				if(orientation == VERTICAL){
					grab = event->getY() - offsetY - yPos;
				}else{
					grab = event->getX() - offsetX - xPos;
				}

				update();
				event->setControl(this);
				return true;

			}else if(base->boundsCheck(event->getX(), event->getY())){

				setActive(true);

				if(orientation == VERTICAL){
					grab = event->getY() - offsetY - yPos;

					sliderTop->setY(grab - (int)sliderSize/2);

					if(sliderTop->getY() < padding){
						sliderTop->setY(padding);
					}else if(sliderTop->getY() + sliderSize > getHeight() - padding){
						sliderTop->setY(getHeight() - padding - sliderSize);
					}

					sliderMid->setY(sliderTop->getY() + sliderTop->getRadius());
					sliderBottom->setY(sliderMid->getY() + sliderMid->getHeight());
				}else{
					grab = event->getX() - offsetX - xPos;

					sliderTop->setX(grab - (int)sliderSize/2);

					if(sliderTop->getX() < padding){
						sliderTop->setX(padding);
					}else if(sliderTop->getX() + sliderSize > getWidth() - padding){
						sliderTop->setX(getWidth() - padding - sliderSize);
					}

					sliderMid->setX(sliderTop->getX() + sliderTop->getRadius());
					sliderBottom->setX(sliderMid->getX() + sliderMid->getWidth());
				}

				update();
				event->setControl(this);
				return true;
			}

		}

	//Handle mouse release event
	}else if((event->getType() == e_MOUSEPRESS && event->getMouseState() == s_UP ||
						event->getType() == e_CLICK) && getActive()){

		if(event->getButton() == b_LEFT){

			setActive(false);
			update();
			event->setControl(this);
			return true;
		}

	//Handle mouse move event
	}else if(event->getType() == e_DRAG && getActive()){

		if(orientation == VERTICAL){
			int adjustedY = event->getY() - offsetY - yPos;


			sliderTop->setY(sliderTop->getY() + (adjustedY - grab));

			if(sliderTop->getY() < padding){
				sliderTop->setY(padding);
			}else if(sliderTop->getY() + sliderSize > getHeight() - padding){
				sliderTop->setY(getHeight() - padding - sliderSize);
			}else{
				grab = adjustedY;
			}

			sliderMid->setY(sliderTop->getY() + sliderTop->getRadius());
			sliderBottom->setY(sliderMid->getY() + sliderMid->getHeight());
		}else{
			int adjustedX = event->getX() - offsetX - xPos;


			sliderTop->setX(sliderTop->getX() + (adjustedX - grab));

			if(sliderTop->getX() < padding){
				sliderTop->setX(padding);
			}else if(sliderTop->getX() + sliderSize > getWidth() - padding){
				sliderTop->setX(getWidth() - padding - sliderSize);
			}else{
				grab = adjustedX;
			}

			sliderMid->setX(sliderTop->getX() + sliderTop->getRadius());
			sliderBottom->setX(sliderMid->getX() + sliderMid->getWidth());
		}

		update();
		event->setControl(this);
		return true;
	}

	return false;
}

/*update the theme of the control*/
bool ScrollBar::updateTheme(){
	if(darkMode){
		base->setFillColor(*darkTheme[4]);

		if(getActive()){
			sliderTop->setFillColor(*accent);
			sliderMid->setFillColor(*accent);
			sliderBottom->setFillColor(*accent);
		}else{
			sliderTop->setFillColor(*darkTheme[3]);
			sliderMid->setFillColor(*darkTheme[3]);
			sliderBottom->setFillColor(*darkTheme[3]);
		}

	}else{
		base->setFillColor(*lightTheme[4]);

		if(getActive()){
			sliderTop->setFillColor(*accent);
			sliderMid->setFillColor(*accent);
			sliderBottom->setFillColor(*accent);
		}else{
			sliderTop->setFillColor(*lightTheme[3]);
			sliderMid->setFillColor(*lightTheme[3]);
			sliderBottom->setFillColor(*lightTheme[3]);
		}

	}
	
	redraw = true;
	return true;
}

/*free all the memory used*/
void ScrollBar::free(){
	base->free();
	sliderMid->free();
	sliderTop->free();
	sliderBottom->free();

	delete base;
	delete sliderMid;
	delete sliderTop;
	delete sliderBottom;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the ScrollBar*/
bool ScrollBar::boundsCheck(int x, int y){
	x -= offsetX;
	y -= offsetY;

	return x >= getX() && x < getX() + getWidth() && y >= getY() && y < getY() + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void ScrollBar::update(){
	if(getActive()){
		sliderTop->setFillColor(*accent);
		sliderMid->setFillColor(*accent);
		sliderBottom->setFillColor(*accent);
	}else if(darkMode){
		sliderTop->setFillColor(*darkTheme[3]);
		sliderMid->setFillColor(*darkTheme[3]);
		sliderBottom->setFillColor(*darkTheme[3]);
	}else{
		sliderTop->setFillColor(*lightTheme[3]);
		sliderMid->setFillColor(*lightTheme[3]);
		sliderBottom->setFillColor(*lightTheme[3]);
	}

	redraw = true;
}

/*update the texture of the ScrollBar to be printed later*/
bool ScrollBar::updateTexture(SDL_Renderer *renderer){
	base->draw(renderer);

	sliderTop->draw(renderer);
	sliderMid->draw(renderer);
	sliderBottom->draw(renderer);

	return true;
}
