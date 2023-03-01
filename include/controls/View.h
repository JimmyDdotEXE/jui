#ifndef VIEW_H
#define VIEW_H

#include <vector>
#include <string>

#include "objects/Rectangle.h"
#include "controls/ScrollBar.h"
#include "controls/Control.h"


class View : public Control{
	public:
		View(int x, int y, uint w, uint h);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setViewWidth(uint w);
		bool setViewHeight(uint h);
		bool setVerticalLock(bool b);
		bool setHorizontalLock(bool b);

		bool clear(Color color=Color());

		bool drawObject(DrawObject *object);

		bool addControl(Control *control);
		bool removeControl(Control *control);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		bool removeHorizontalBar();
		bool removeVerticalBar();

		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		uint width;
		uint height;

		int viewX;
		int viewY;
		uint viewWidth;
		uint viewHeight;
		SDL_Texture *view;

		Color clearColor;

		bool verticalLock;
		bool horizontalLock;

		ScrollBar *verticalBar;
		ScrollBar *horizontalBar;
		Rectangle *scrollCorner;

		std::vector<DrawObject *> drawQueue;
		std::vector<Control *> controls;
};

#endif
