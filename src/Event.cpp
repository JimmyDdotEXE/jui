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
Event::Event(int x, int y, EVENT_Button b, EVENT_State s, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_MOUSEPRESS;

	button = b;
	mouseState = s;

	mod = m_NONE;

	text = "";

	xPos = x;
	yPos = y;

	previousX = 0;
	previousY = 0;

	control = NULL;

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
Event::Event(int x, int y, EVENT_Button b, EVENT_Click c, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_CLICK;

	button = b;
	click = c;

	mod = m_NONE;

	text = "";

	xPos = x;
	yPos = y;

	previousX = 0;
	previousY = 0;

	control = NULL;

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
Event::Event(int x, int y, int oldX, int oldY, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_MOVE;

	mod = m_NONE;

	text = "";

	xPos = x;
	yPos = y;

	previousX = oldX;
	previousY = oldY;

	control = NULL;

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
Event::Event(int x, int y, int oldX, int oldY, EVENT_Button b, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_DRAG;

	button = b;
	mouseState = s_DOWN;

	mod = m_NONE;

	text = "";

	xPos = x;
	yPos = y;

	previousX = oldX;
	previousY = oldY;

	control = NULL;

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
Event::Event(int x, int y, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_HOVER;

	mod = m_NONE;

	text = "";

	xPos = x;
	yPos = y;

	previousX = 0;
	previousY = 0;

	control = NULL;

	tail = t;
}

/*
	constructs HOLD Event
	int x: X coordinate of the event
	int y: Y coordinate of the event
	EVENT_Button b: mouse button being held in the event
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(int x, int y, EVENT_Button b, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_HOLD;

	button = b;
	mouseState = s_DOWN;

	mod = m_NONE;

	text = "";

	xPos = x;
	yPos = y;

	previousX = 0;
	previousY = 0;

	control = NULL;

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
Event::Event(int x, int y, EVENT_ScrollDirection s, EVENT_Modifier m, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_SCROLL;

	scroll = s;

	mod = m;

	text = "";

	xPos = x;
	yPos = y;

	previousX = 0;
	previousY = 0;

	control = NULL;

	tail = t;
}

/*
	constructs KEYPRESS Event
	SDL_Keycode k: key causing the event
	EVENT_State s: state of the key in the event
	EVENT_Modifier m: which mod key is being held
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(SDL_Keycode k, EVENT_State s, EVENT_Modifier m, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_KEYPRESS;

	key = k;
	keyState = s;
	mod = m;

	text = "";

	xPos = 0;
	yPos = 0;

	previousX = 0;
	previousY = 0;

	control = NULL;

	tail = t;
}

/*
	constructs TEXTENTRY Event
	std::string s: text that has been entered
	Event *t: previous Event that will be the tail of a linked list
 */
Event::Event(std::string s, Event *t){
	timeStamp = SDL_GetTicks();

	type = e_TEXTENTRY;

	mod = m_NONE;

	text = s;

	xPos = 0;
	yPos = 0;

	previousX = 0;
	previousY = 0;

	control = NULL;

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

/*get the time stamp of the event*/
uint Event::getTimeStamp(){
	return timeStamp;
}

/*get the control that claims the event*/
Control *Event::getControl(){
	return control;
}

/*get the previous event that makes up the tail of the current event*/
Event *Event::getTail(){
	return tail;
}


/*set the control that claims the event*/
bool Event::setControl(Control *c){
	control = c;

	return control == c;
}

/*set the previous event that makes up the tail of the current event*/
bool Event::setTail(Event *t){
	tail = t;

	return tail == t;
}
