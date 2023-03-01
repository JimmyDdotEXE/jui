#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <vector>
#include <string>
#include <dirent.h>

#include "controls/ListView.h"
#include "controls/TextBox.h"
#include "controls/Label.h"
#include "controls/ComboBox.h"
#include "controls/Control.h"

class DirectoryView : public Control{
	public:
		DirectoryView(int x, int y, uint w, uint h, std::string entry, std::string exten);
		DirectoryView(int x, int y, uint w, uint h, std::string entry, std::vector<std::string> exten={});

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		std::string getSelection();

		virtual bool setX(double x);
		virtual bool setY(double y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);

		virtual bool handleEvent(Event *event);

		virtual bool updateTheme();
		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual void update();
		virtual bool updateTexture(SDL_Renderer *renderer);

		bool loadDirectory(std::string dir);

		std::string oldDirName;

		std::string dirString;
		std::string selection;
		std::vector<std::string> extensions;
		DIR *currentDir;
		
		std::vector<std::string> entryNames;

		bool selectDirMode;
		bool selectAnyMode;
		bool loadMode;

		bool filter;

		ListView *view;
		TextBox *search;
		Label *label;
};

#endif
