#include "Window.h"
#include "controls/Control.h"


/*
	Control constructor
	initializes all Control variables
*/
Control::Control(){
	active = false;
	focus = false;

	visible = true;

	window = NULL;

	lockLeft = false;
	lockRight = false;
	lockTop = false;
	lockBottom = false;

	locksForceResize = false;

	ignoreMouseDeadZone = false;

	activeControl = NULL;
}


bool Control::getActive(){
	return active;
}

/*is the control visible or not*/
bool Control::getVisible(){
	return visible;
}

/*
	getLeftLock function
	returns boolean that tells if the control is locked to the left of the view
*/
bool Control::getLeftLock(){
	return lockLeft;
}

/*
	getRightLock function
	returns boolean that tells if the control is locked to the right of the view
*/
bool Control::getRightLock(){
	return lockRight;
}

/*
	getTopLock function
	returns boolean that tells if the control is locked to the top of the view
*/
bool Control::getTopLock(){
	return lockTop;
}

/*
	getBottomLock function
	returns boolean that tells if the control is locked to the bottom of the view
*/
bool Control::getBottomLock(){
	return lockBottom;
}

/*
	getLockResize function
	returns boolean that tells if opposing locks resize the control
*/
bool Control::getLockResize(){
	return locksForceResize;
}

/*get the window the control is being managed by*/
Window *Control::getWindow(){
	return window;
}

bool Control::setActive(bool b){
	active = b;

	if(!b && activeControl != NULL){
		activeControl->setActive(b);
		activeControl = NULL;
	}

	return active == b;
}


/*set if the control is visible*/
bool Control::setVisible(bool b){
	visible = b;

	return visible == b;
}

/*set of the control is locked to the left of the view*/
bool Control::setLeftLock(bool b){
	lockLeft = b;

	return lockLeft == b;
}

/*set of the control is locked to the right of the view*/
bool Control::setRightLock(bool b){
	lockRight = b;

	return lockRight == b;
}

/*set of the control is locked to the top of the view*/
bool Control::setTopLock(bool b){
	lockTop = b;

	return lockTop == b;
}

/*set of the control is locked to the bottom of the view*/
bool Control::setBottomLock(bool b){
	lockBottom = b;

	return lockBottom == b;
}

/*set if opposing edge locks force the control to resize on view resize*/
bool Control::setLockResize(bool b){
	locksForceResize = b;

	return locksForceResize == b;
}

/*set the window that is managing the control*/
bool Control::setWindow(Window *win){
	window = win;

	return window == win;
}


/*returns if the control needs the mouse deadzone to be ignored or not*/
bool Control::isDeadZoneIgnored(){
	return ignoreMouseDeadZone;
}




Control *Control::getActiveControl(){
	return activeControl;
}


bool Control::setActiveControl(Control *c){
	if(activeControl != NULL && activeControl != c){
		activeControl->setActive(false);
	}

	activeControl = c;
	return activeControl == c;
}
