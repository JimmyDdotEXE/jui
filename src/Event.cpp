#include <iostream>
#include "controls/Control.h"
#include "Event.h"


/*
	constructs MOUSEPRESS Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	EVENT_Button b: mouse button involved in the event
	EVENT_State s: state of the button for the event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, EVENT_Button b, EVENT_State s, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_MOUSEPRESS;

	button = b;
	mouseState = s;

	xPos = x;
	yPos = y;

	tail = t;
}

/*
	constructs CLICK Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	EVENT_Button b: mouse button involved in the event
	EVENT_Click c: number of clicks for the event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, EVENT_Button b, EVENT_Click c, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_CLICK;

	button = b;
	click = c;

	xPos = x;
	yPos = y;

	tail = t;
}

/*
	constructs MOVE Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	int oldX: X coordinate of the previous event
	int oldY: Y coordinate of the previous event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, int oldX, int oldY, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_MOVE;

	xPos = x;
	yPos = y;

	previousX = oldX;
	previousY = oldY;

	tail = t;
}

/*
	constructs DRAG Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	int oldX: X coordinate of the previous event
	int oldY: Y coordinate of the previous event
	EVENT_Button b: mouse button being held in the event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, int oldX, int oldY, EVENT_Button b, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_DRAG;

	button = b;

	xPos = x;
	yPos = y;

	previousX = oldX;
	previousY = oldY;

	tail = t;
}

/*
	constructs HOVER Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_HOVER;

	xPos = x;
	yPos = y;

	tail = t;
}

/*
	constructs HOLD Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	EVENT_Button b: mouse button being held in the event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, EVENT_Button b, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_HOLD;

	button = b;
	mouseState = s_DOWN;

	mod = m_NONE;

	xPos = x;
	yPos = y;

	tail = t;
}

/*
	constructs SCROLL Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	EVENT_ScrollDirection s: direction that is being scrolled
	EVENT_Modifier m: which mod key is being held
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, EVENT_ScrollDirection s, EVENT_Modifier m, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_SCROLL;

	scroll = s;

	mod = m;

	xPos = x;
	yPos = y;

	tail = t;
}

/*
	constructs KEYPRESS Event
	SDL_Keycode k: key causing the event
	EVENT_State s: state of the key in the event
	EVENT_Modifier m: which mod key is being held
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(SDL_Keycode k, EVENT_State s, EVENT_Modifier m, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_KEYPRESS;

	key = k;
	keyState = s;
	mod = m;

	tail = t;
}

/*
	constructs TEXTENTRY Event
	std::string s: text that has been entered
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(std::string s, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_TEXTENTRY;

	text = s;

	tail = t;
}

Event::Event(EVENT_Window w, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_WINDOW;

	windowEvent = w;

	tail = t;
}

Event::Event(EVENT_Window w, int data1, int data2, uint time, int winID, Event *t){
	timeStamp = time;
	windowID = winID;

	type = e_WINDOW;

	windowEvent = w;

	switch(w){
		case w_MOVED:
			xPos = data1;
			yPos = data2;
			break;
		case w_RESIZED:
			width = data1;
			height = data2;
			break;
		case w_SIZECHANGED:
			width = data1;
			height = data2;
			break;
		default:
			break;
	}

	tail = t;
}


/*get the type of the event*/
EVENT_EventType Event::getType(){
	return type;
}

/*get the button involved the event*/
EVENT_Button Event::getButton(){
	return button;
}

/*get the number of clicks of the event*/
EVENT_Click Event::getClick(){
	return click;
}

/*get the state of the mouse button from the event*/
EVENT_State Event::getMouseState(){
	return mouseState;
}

/*get the scroll direction of the event*/
EVENT_ScrollDirection Event::getScrollDirection(){
	return scroll;
}

/*get the key involved in the event*/
SDL_Keycode Event::getKey(){
	return key;
}

/*get the state of the key from the event*/
EVENT_State Event::getKeyState(){
	return keyState;
}

/*get the modifier key being held during the event*/
EVENT_Modifier Event::getModifier(){
	return mod;
}

EVENT_Window Event::getWindowEvent(){
	return windowEvent;
}

/*get the text entered in the event*/
std::string Event::getText(){
	return text;
}

/*get the X position of the event*/
int Event::getX(){
	return xPos;
}

/*get the Y position of the event*/
int Event::getY(){
	return yPos;
}

/*get the previous X position of the event*/
int Event::getPreviousX(){
	return previousX;
}

/*get the previous Y position of the event*/
int Event::getPreviousY(){
	return previousY;
}

int Event::getWidth(){
	return width;
}

int Event::getHeight(){
	return height;
}

/*get the time stamp of the event*/
uint Event::getTimeStamp(){
	return timeStamp;
}

int Event::getWindowID(){
	return windowID;
}

/*get the control that claims the event*/
Control *Event::getControl(){
	if(controls.size() > 0){
		return controls.at(0);
	}

	return NULL;
}

std::vector<Control *> Event::getControls(){
	return controls;
}

/*get the previous event that makes up the tail of the current event*/
Event *Event::getTail(){
	return tail;
}


/*set the control that claims the event*/
bool Event::setControl(Control *c){
	controls.insert(controls.begin(), c);

	return controls.at(0) == c;
}

/*set the previous event that makes up the tail of the current event*/
//TODO: evaluate if getTail is needed
bool Event::setTail(Event *t){
	tail = t;

	return tail == t;
}
