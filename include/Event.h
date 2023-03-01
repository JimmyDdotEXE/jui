#ifndef EVENT_H
#define EVENT_H

#include <vector>
#include <SDL2/SDL.h>
#include "Utility.h"
class Control;


/*
	EVENT_EventType enumeration 

	e_MOUSEPRESS:	mouse button has either been pressed or been released
	e_CLICK: mouse button has being clicked
	e_MOVE: mouse was being moved
	e_DRAG: mouse was being moved with a button pressed
	e_HOVER: mouse was hover in place
	e_HOLD: mouse was hovering with a button pressed
	e_SCROLL: mouse wheel was scrolled
	e_KEYPRESS: a keyboard key has either been pressed or been released
	e_TEXTENTRY: text was typed
 */
typedef enum{
	e_MOUSEPRESS,
	e_CLICK,
	e_MOVE,
	e_DRAG,
	e_HOVER,
	e_HOLD,
	e_SCROLL,
	e_KEYPRESS,
	e_TEXTENTRY,
	e_WINDOW
} EVENT_EventType;

/*
	EVENT_Button enumeration 

	b_LEFT:	the left mouse button
	b_RIGHT:the right mouse button 
	b_MIDDLE:the middle mouse button 
 */
typedef enum{
	b_LEFT,
	b_RIGHT,
	b_MIDDLE
} EVENT_Button;

/*
	EVENT_Click enumeration 

	b_SINGLE: single click
	b_DOUBLE: double click
	b_TRIPLE: triple click
 */
typedef enum{
	c_SINGLE,
	c_DOUBLE,
	c_TRIPLE
} EVENT_Click;

/*
	EVENT_ScrollDirection enumeration 

	w_UP:	mouse wheel scrolled up
	w_DOWN: mouse wheel scrolled down
	w_LEFT: mouse wheel scrolled left
	w_RIGHT: mouse wheel scrolled right
 */
typedef enum{
	w_UP,
	w_DOWN,
	w_LEFT,
	w_RIGHT
} EVENT_ScrollDirection;

/*
	EVENT_State enumeration 

	s_UP:	key or mouse button released
	s_DOWN: key or mouse button pressed
 */
typedef enum{
	s_UP,
	s_DOWN
} EVENT_State;

/*
	EVENT_Modifier enumeration 

	m_NONE:	no mod key pressed
	m_SHIFT: Shift key
	m_CONTROL: Control key
	m_ALT: Alt key
 */
typedef enum{
	m_NONE,
	m_SHIFT,
	m_CONTROL,
	m_ALT
} EVENT_Modifier;

typedef enum{
	w_SHOWN,
	w_HIDDEN,
	w_EXPOSED,
	w_MOVED,
	w_RESIZED,
	w_SIZECHANGED,
	w_MINIMIZED,
	w_MAXIMIZED,
	w_RESTORED,
	w_ENTER,
	w_LEAVE,
	w_FOCUSGAINED,
	w_FOCUSLOST,
	w_CLOSE
} EVENT_Window;


class Event{
	public:
		Event(int x, int y, EVENT_Button b, EVENT_State s, uint time, int winID, Event *t=NULL);
		Event(int x, int y, EVENT_Button b, EVENT_Click c, uint time, int winID, Event *t=NULL);
		Event(int x, int y, int oldX, int oldY, uint time, int winID, Event *t=NULL);
		Event(int x, int y, int oldX, int oldY, EVENT_Button b, uint time, int winID, Event *t=NULL);
		Event(int x, int y, uint time, int winID, Event *t=NULL);
		Event(int x, int y, EVENT_Button b, uint time, int winID, Event *t=NULL);
		Event(int x, int y, EVENT_ScrollDirection s, EVENT_Modifier m, uint time, int winID, Event *t=NULL);
		Event(SDL_Keycode k, EVENT_State s, EVENT_Modifier m, uint time, int winID, Event *t=NULL);
		Event(std::string s, uint time, int winID, Event *t=NULL);
		Event(EVENT_Window w, uint time, int winID, Event *t=NULL);
		Event(EVENT_Window w, int data1, int data2, uint time, int winID, Event *t=NULL);

		EVENT_EventType getType();
		EVENT_Button getButton();
		EVENT_Click getClick();
		EVENT_State getMouseState();
		EVENT_ScrollDirection getScrollDirection();
		SDL_Keycode getKey();
		EVENT_State getKeyState();
		EVENT_Modifier getModifier();
		EVENT_Window getWindowEvent();
		std::string getText();
		int getX();
		int getY();
		int getPreviousX();
		int getPreviousY();
		int getWidth();
		int getHeight();
		uint getTimeStamp();
		int getWindowID();
		Control *getControl();
		std::vector<Control *> getControls();
		Event *getTail();

		bool setControl(Control *c);
		bool setTail(Event *t);


	private:
		EVENT_EventType type;

		EVENT_Button button;
		EVENT_Click click;
		EVENT_State mouseState;

		EVENT_ScrollDirection scroll;

		SDL_Keycode key;
		EVENT_State keyState;
		EVENT_Modifier mod;

		EVENT_Window windowEvent;

		std::string text;

		int xPos;
		int yPos;

		int previousX;
		int previousY;

		int width;
		int height;

		uint timeStamp;

		int windowID;

		std::vector<Control *> controls;

		Event *tail;
};

#endif
