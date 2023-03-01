#include <iostream>
#include <cmath>
#include "Utility.h"
#include "controls/DirectoryView.h"


DirectoryView::DirectoryView(int x, int y, uint w, uint h, std::string entry, std::string exten){
	dirString = entry;
	extensions = {stringLower(exten)};

	currentDir = NULL;

	selectDirMode = exten == "/";
	selectAnyMode = exten == "*";
	loadMode = false;

	filter = false;

	label = new Label(0, 0, 12, "." + exten);
	label->setPadding(label->getHeight()/2);
	label->setX(w - label->getWidth());

	search = new TextBox(0, 0, label->getX() - 6, 24, "search");

	view = new ListView(0, 0, w, h - search->getHeight() - 6, extensions, 12, [&dirString=dirString, &selection=selection](std::string choice)
			{
			if(isDirectory(dirString + '/' + choice)){
				if(choice == "../"){
					std::size_t pos = dirString.find_last_of('/');

					if(pos == 0){
						dirString = "/";
					}else if(pos != std::string::npos){
						dirString = dirString.substr(0, pos);
					}else{
						return false;
					}
				}else{
					dirString = dirString + '/' + choice.substr(0, choice.length()-1);
				}

				selection = "";
			}else{
				return false;
			}

			return true;
			});

	label->setY(view->getHeight() + 6);
	search->setY(view->getHeight() + 6);

	loadDirectory(entry);

	setX(x);
	setY(y);

	updateTheme();
	textureLock = false;
}

DirectoryView::DirectoryView(int x, int y, uint w, uint h, std::string entry, std::vector<std::string> exten){
	dirString = entry;

	for(int i=0;i<exten.size();i++){
		extensions.push_back(stringLower(exten.at(i)));
	}

	currentDir = NULL;

	selectDirMode = false;
	selectAnyMode = exten.size() == 0;
	loadMode = true;

	filter = false;

	label = NULL;

	search = new TextBox(0, 0, w, 24, "search");

	view = new ListView(0, 0, w, h - search->getHeight() - 6, extensions, 12, [&dirString=dirString, &selection=selection](std::string choice)
			{
			if(isDirectory(dirString + '/' + choice)){
				if(choice == "../"){
					std::size_t pos = dirString.find_last_of('/');

					if(pos == 0){
						dirString = "/";
					}else if(pos != std::string::npos){
						dirString = dirString.substr(0, pos);
					}else{
						return false;
					}
				}else{
					dirString = dirString + '/' + choice.substr(0, choice.length()-1);
				}

				selection = "";
			}else{
				return false;
			}

			return true;
			});

	search->setY(view->getHeight() + 6);

	loadDirectory(entry);

	setX(x);
	setY(y);

	updateTheme();
	textureLock = false;
}


int DirectoryView::getWidth(){
	return view->getWidth();
}

int DirectoryView::getHeight(){
	return search->getY() + search->getHeight();
}

int DirectoryView::getTotalWidth(){
	return getWidth();
}

int DirectoryView::getTotalHeight(){
	return getHeight();
}

std::string DirectoryView::getSelection(){
	if(label != NULL && selection == ""){
		return search->getText() + label->getText();
	}else{
		return dirString + '/' + selection;
	}
}


bool DirectoryView::setX(double x){
	view->setOffsetX(offsetX + x);
	search->setOffsetX(offsetX + x);

	if(label != NULL){
		label->setOffsetX(offsetX + x);
	}

	xPos = x;
	return xPos == x;
}

bool DirectoryView::setY(double y){
	view->setOffsetY(offsetY + y);
	search->setOffsetY(offsetY + y);

	if(label != NULL){
		label->setOffsetY(offsetY + y);
	}

	yPos = y;
	return yPos == y;
}

bool DirectoryView::setWidth(uint w){
	if(label != NULL){
		label->setX(label->getX() + (w-getWidth()));
	}

	search->setWidth(search->getWidth() + (w-getWidth()));

	return view->setWidth(w);
}

bool DirectoryView::setHeight(uint h){
	view->setHeight(view->getHeight() + (h-getHeight()));

	if(label != NULL){
		label->setY(label->getY() + (h-getHeight()));
	}

	return search->setY(search->getY() + (h-getHeight()));
}


bool DirectoryView::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	std::string txt = search->getText();

	bool ret = false;
	Control *aControl = getActiveControl();

	if(aControl != NULL){
		aControl->handleEvent(event);

		if(event->getControl() == aControl){
			if(!aControl->getActive()){
				setActive(false);
			}

			filter = txt != search->getText();

			update();
			event->setControl(this);
			ret = true;
		}
	}

	if(aControl != view){
		view->handleEvent(event);

		if(event->getControl() == view){
			if(view->getActive()){
				setActive(true);
				setActiveControl(view);
			}else{
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}
	}

	if(aControl != search){
		search->handleEvent(event);

		if(event->getControl() == search){
			if(search->getActive()){
				setActive(true);
				setActiveControl(search);
			}else{
				setActive(false);
			}

			filter = txt != search->getText();

			update();
			event->setControl(this);
			ret = true;
		}
	}

	return ret;
}


bool DirectoryView::updateTheme(){
	view->updateTheme();
	search->updateTheme();

	if(label != NULL){
		label->updateTheme();
	}

	return true;
}

void DirectoryView::free(){
	if(currentDir != NULL){
		closedir(currentDir);
		currentDir = NULL;
	}

	if(view != NULL){
		view->free();
		delete view;
		view = NULL;
	}

	if(search != NULL){
		search->free();
		delete search;
		search = NULL;
	}

	if(label != NULL){
		label->free();
		delete label;
		label = NULL;
	}

	setTexture(NULL);
}

bool DirectoryView::boundsCheck(int x, int y){
	x -= offsetX;
	y -= offsetY;

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




void DirectoryView::update(){
	if(dirString != oldDirName){
		loadDirectory(dirString);
	}

	std::vector<std::string> selected = view->getSelection();
	if(selected.size() > 0){
		selection = selected.at(0);
	}

	if(filter){
		std::string txt = search->getText();
		std::size_t pos = txt.find_last_of('/');

		if(dirString != txt.substr(0, pos)){
			loadDirectory(txt.substr(0, pos));
			search->setText(search->getText()+txt.substr(pos+1));
		}

		view->filter(txt.substr(pos+1));

		filter = false;
	}

	redraw = true;
}

bool DirectoryView::updateTexture(SDL_Renderer *renderer){
	view->draw(renderer);
	search->draw(renderer);

	if(label != NULL){
		label->draw(renderer);
	}

	return true;
}


bool DirectoryView::loadDirectory(std::string dir){
	if(currentDir != NULL){
		closedir(currentDir);
		currentDir = NULL;
	}

	entryNames.clear();

	std::vector<std::string> dirs;
	std::vector<std::string> files;

	if(currentDir = opendir(dir.c_str())){
		struct dirent *entry = NULL;
		while(entry = readdir(currentDir)){
			std::string entryName = std::string(entry->d_name);

			if(isDirectory(dir + "/" + entryName)){
				if(entryName != "."){
					dirs.push_back(entryName + "/");
				}
			}else{
				std::size_t pos = entryName.find_last_of('.');
				std::string exten = "";

				if(pos != std::string::npos){
					exten = stringLower(entryName.substr(pos+1));
				}

				if(std::find(extensions.begin(), extensions.end(), exten) != extensions.end() || selectAnyMode){
					files.push_back(entryName);
				}
			}
		}

		sortAlpha(&dirs);
		sortAlpha(&files);

		entryNames.insert(entryNames.end(), dirs.begin(), dirs.end());
		entryNames.insert(entryNames.end(), files.begin(), files.end());

		view->setList(entryNames);

		dirString = dir;
		oldDirName = dir;
		search->setText(dir + '/');
		return true;
	}else{
		std::cout << "couldn't open directory " << currentDir << std::endl;
	}

	return false;
}
