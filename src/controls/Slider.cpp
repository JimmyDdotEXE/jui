#include <iostream>
#include "controls/Slider.h"


/*
	Slider constructor

	int x: X position of the slider
	int y: Y position of the slider
	uint size: size of the slider
	double mi: minimum value allowed on the slider
	double ma: maximum value allowed on the slider
	double *n: pointer to the double the slider controls
	e_Orien orien: orientation of the slider
 */
Slider::Slider(int x, int y, uint size, double mi, double ma, double *n, e_Orien orien){
	orientation = orien;

	if(orientation == HORIZONTAL){
		edgeLeft = new SemiCircle(4, 4, 4);
		edgeLeft->setRotation(90);
		edgeRight = new SemiCircle(8+size, 4, 4);
		edgeRight->setRotation(270);
		baseLeft = new Rectangle(8, 4, 0, 8);
		baseRight = new Rectangle(8, 4, size, 8);
		sliderBase = new Circle(0, 0, 8);
		sliderMid = new Circle(4, 4, 4);
	}else{
		edgeLeft = new SemiCircle(4, 8+size, 4);
		edgeRight = new SemiCircle(4, 4, 4);
		edgeRight->setRotation(180);
		baseLeft =	new Rectangle(4, 8+size, 8, 0);
		baseRight = new Rectangle(4, 8, 8, size);
		sliderBase = new Circle(0, size, 8);
		sliderMid = new Circle(4, 4+size, 4);
	}

	target = n;
	min = mi;
	max = ma;
	incrementSize = (double)(ma-mi)/(double)size;

	setX(x);
	setY(y);

	updateTheme();
	update();
}


/*get the width*/
int Slider::getWidth(){
	if(orientation == VERTICAL){
		return sliderBase->getRadius()*2;
	}else{
		return sliderBase->getRadius()*2 + baseRight->getWidth();
	}
}

/*get the height*/
int Slider::getHeight(){
	if(orientation == HORIZONTAL){
		return sliderBase->getRadius()*2;
	}else{
		return sliderBase->getRadius()*2 + baseRight->getHeight();
	}
}

/*get the total width*/
int Slider::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Slider::getTotalHeight(){
	return getHeight();
}


/*set the X position*/
bool Slider::setX(double x){
	edgeLeft->setOffsetX(offsetX + x);
	edgeRight->setOffsetX(offsetX + x);
	baseLeft->setOffsetX(offsetX + x);
	baseRight->setOffsetX(offsetX + x);
	sliderBase->setOffsetX(offsetX + x);
	sliderMid->setOffsetX(offsetX + x);

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Slider::setY(double y){
	edgeLeft->setOffsetY(offsetY + y);
	edgeRight->setOffsetY(offsetY + y);
	baseLeft->setOffsetY(offsetY + y);
	baseRight->setOffsetY(offsetY + y);
	sliderBase->setOffsetY(offsetY + y);
	sliderMid->setOffsetY(offsetY + y);

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool Slider::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool Slider::setHeight(uint h){
	return false;
}


/*try to handle the given event*/
bool Slider::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	uint temp = SDL_GetTicks();

	//Handle scroll wheel event
	if(event->getType() == e_SCROLL){
		if(boundsCheck(event->getX(), event->getY())){
			if(event->getScrollDirection() == w_UP){
				if(*target + incrementSize > *target && *target + incrementSize <= max){
					*target += incrementSize;
				}else{
					*target = max;
				}


				update();
				event->setControl(this);
				return true;
			}else if(event->getScrollDirection() == w_DOWN){

				if(*target - incrementSize < *target && *target - incrementSize >= min){
					*target -= incrementSize;
				}else{
					*target = min;
				}


				update();
				event->setControl(this);
				return true;
			}
		}

	//Handle mouse pressed event
	}else if(event->getType() == e_MOUSEPRESS){

		if(event->getButton() == b_LEFT && event->getMouseState()){

			if(sliderBase->boundsCheck(event->getX(), event->getY())){
				setActive(true);

				if(orientation == HORIZONTAL){
					grab = event->getX();
				}else{
					grab = event->getY();
				}

				update();
				event->setControl(this);

			}else if(baseRight->boundsCheck(event->getX(), event->getY())){
				setActive(true);


				if(orientation == HORIZONTAL){
					*target = (event->getX() - offsetX - xPos - baseRight->getX()) * incrementSize;

					grab = event->getX();
				}else{
					*target = (baseRight->getHeight() - (event->getY() - offsetY - yPos - baseRight->getY())) * incrementSize;

					grab = event->getY();
				}

				update();
				event->setControl(this);
			}else if(edgeLeft->boundsCheck(event->getX(), event->getY())){
				setActive(true);
				*target = min;


				if(orientation == HORIZONTAL){
					grab = event->getX();
				}else{
					grab = event->getY();
				}

				update();
				event->setControl(this);
			}else if(edgeRight->boundsCheck(event->getX(), event->getY())){
				setActive(true);
				*target = max;


				if(orientation == HORIZONTAL){
					grab = event->getX();
				}else{
					grab = event->getY();
				}

				update();
				event->setControl(this);
			}else{
				setActive(false);
				update();
				return true;
			}

			return true;
		}

	//Handle mouse release event
	}else if(event->getType() == e_HOVER && getActive()){
			setActive(false);
			update();
			return true;

	//Handle mouse move event
	}else if(event->getType() == e_DRAG && getActive()){

		if(orientation == HORIZONTAL){

			if((event->getX() < grab && *target > min) || (event->getX() > grab && *target < max)){
				double temp = *target + (double)(event->getX() - grab) * incrementSize;

				if(temp > max){
					grab = ((max - *target) / incrementSize) + grab;

					*target = max;
				}else if(temp < min){
					grab = ((min - *target) / incrementSize) + grab;

					*target = min;
				}else{
					grab = event->getX();

					*target = temp;
				}
			}
		}else{

			if((event->getY() > grab && *target > min) || (event->getY() < grab && *target < max)){
				double temp = *target + (double)(event->getY() - grab) * (incrementSize * -1.0);

				if(temp > max){
					grab = ((max - *target) / (incrementSize * -1.0)) + grab;

					*target = max;
				}else if(temp < min){
					grab = ((min - *target) / (incrementSize * -1.0)) + grab;

					*target = min;
				}else{
					grab = event->getY();

					*target = temp;
				}
			}
		}

		update();
		event->setControl(this);
		return true;
	}

	return false;
}


/*update the theme of the control*/
bool Slider::updateTheme(){

	if(darkMode){
		edgeLeft->setFillColor(*accent);
		edgeRight->setFillColor(*darkTheme[4]);
		baseLeft->setFillColor(*accent);
		baseRight->setFillColor(*darkTheme[4]);
		sliderBase->setFillColor(*darkTheme[3]);
		sliderMid->setFillColor(*darkTheme[4]);
	}else{
		edgeLeft->setFillColor(*accent);
		edgeRight->setFillColor(*lightTheme[4]);
		baseLeft->setFillColor(*accent);
		baseRight->setFillColor(*lightTheme[4]);
		sliderBase->setFillColor(*lightTheme[3]);
		sliderMid->setFillColor(*lightTheme[4]);
	}

	redraw = true;
	return true;
}

/*free all the memory used*/
void Slider::free(){
	edgeLeft->free();
	edgeRight->free();
	baseLeft->free();
	baseRight->free();
	sliderBase->free();
	sliderMid->free();

	delete edgeLeft;
	delete edgeRight;
	delete baseLeft;
	delete baseRight;
	delete sliderBase;
	delete sliderMid;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the slider*/
bool Slider::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void Slider::update(){

	if(orientation == HORIZONTAL){
		sliderBase->setX(*target / incrementSize);
		sliderMid->setX(sliderBase->getX() + (sliderBase->getRadius() - sliderMid->getRadius()));
		baseLeft->setWidth(sliderBase->getX());
	}else{
		sliderBase->setY(baseRight->getHeight() - (int)(*target / incrementSize));
		sliderMid->setY(sliderBase->getY() + (sliderBase->getRadius() - sliderMid->getRadius()));
		baseLeft->setHeight(baseRight->getHeight() - sliderBase->getY());
		baseLeft->setY(edgeLeft->getY() - baseLeft->getHeight());
	}


	if(getActive()){
		sliderMid->setFillColor(*accent);
	}else if(darkMode){
		sliderMid->setFillColor(*darkTheme[4]);
	}else if(!darkMode){
		sliderMid->setFillColor(*lightTheme[4]);
	}

	redraw = true;
}

/*update the texture of the slider to be printed later*/
bool Slider::updateTexture(SDL_Renderer *renderer){

	edgeRight->draw(renderer);
	edgeLeft->draw(renderer);
	baseRight->draw(renderer);
	baseLeft->draw(renderer);
	sliderBase->draw(renderer);
	sliderMid->draw(renderer);

	return true;
}
