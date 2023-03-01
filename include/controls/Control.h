#ifndef CONTROL_H
#define CONTROL_H

#include "Event.h"
#include "objects/DrawObject.h"

class Window;

class Control : public DrawObject{
	public:
		Control();

		virtual int getWidth() =0;
		virtual int getHeight() =0;
		virtual int getTotalWidth() =0;
		virtual int getTotalHeight() =0;
		bool getActive();
		bool getVisible();
		bool getLeftLock();
		bool getRightLock();
		bool getTopLock();
		bool getBottomLock();
		bool getLockResize();
		Window *getWindow();

		virtual bool setX(double x) =0;
		virtual bool setY(double y) =0;
		virtual bool setWidth(uint w) =0;
		virtual bool setHeight(uint h) =0;
		bool setActive(bool b);
		bool setVisible(bool b);
		bool setLeftLock(bool b);
		bool setRightLock(bool b);
		bool setTopLock(bool b);
		bool setBottomLock(bool b);
		bool setLockResize(bool b);
		bool setWindow(Window *win);

		bool isDeadZoneIgnored();

		virtual bool handleEvent(Event *event) =0;

		virtual bool updateTheme() =0;
		virtual void free() =0;
		virtual bool boundsCheck(int x, int y) =0;

	protected:
		Control *getActiveControl();

		bool setActiveControl(Control *c);

		virtual void update() =0;
		virtual bool updateTexture(SDL_Renderer *renderer) =0;


		bool visible;

		Window *window;

		bool lockLeft;
		bool lockRight;
		bool lockTop;
		bool lockBottom;

		bool locksForceResize;

		bool ignoreMouseDeadZone;

	private:
		bool active;
		bool focus;

		Control *activeControl;
};

#endif
