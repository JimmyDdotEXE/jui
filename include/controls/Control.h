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
		virtual bool getActive() =0;
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
		bool setVisible(bool b);
		bool setLeftLock(bool b);
		bool setRightLock(bool b);
		bool setTopLock(bool b);
		bool setBottomLock(bool b);
		bool setLockResize(bool b);
		bool setWindow(Window *win);

		virtual bool handleEvent(Event *event) =0;

		virtual bool updateTheme() =0;
		virtual void free() =0;
		virtual bool boundsCheck(int x, int y) =0;

	protected:
		virtual void update() =0;
		virtual bool updateTexture(SDL_Renderer *renderer) =0;


		bool active;
		bool focus;

		bool visible;

		Window *window;

		bool lockLeft;
		bool lockRight;
		bool lockTop;
		bool lockBottom;

		bool locksForceResize;
};

#endif
