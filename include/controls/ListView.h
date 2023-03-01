#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <vector>
#include <string>
#include <functional>

#include "objects/Rectangle.h"
#include "objects/Text.h"
#include "controls/View.h"
#include "controls/Control.h"

typedef enum{GENERAL, FIRST, WORD} e_Search;

typedef struct{
	Text text;
	Rectangle highlight;
} t_Entry;


class ListView : public Control{
	public:
		ListView(int x, int y, uint w, uint h, uint size=12, std::function<bool(std::string)> onDoubleClick=NULL);
		ListView(int x, int y, uint w, uint h, std::vector<std::string> ls, uint size=12, std::function<bool(std::string)> onDoubleClick=NULL);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		std::vector<t_Entry *> getSearchList();
		std::vector<std::string> getSelection();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setFontSize(uint s);
		bool setList(std::vector<std::string> s);
		bool setSelection(std::string s);
		bool setSelection(std::vector<std::string> s);

		bool filter(std::string s, e_Search type=GENERAL);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);


		bool multipleEnabled;

		uint fontSize;

		Rectangle *base;
		View *view;
		std::vector<t_Entry *> selected;
		std::vector<t_Entry *> searchList;
		std::vector<t_Entry> fullList;

		std::function<bool(std::string)> doubleClick;
};

#endif
