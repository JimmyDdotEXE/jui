#include <iostream>
#include <cmath>
#include "controls/TextBox.h"


/*
	TextBox constructor

	int x: X position of the TextBox
	int y: Y position of the TextBox
	uint w: width of the TextBox
	uint h: height of the TextBox
	std::string lab: label of the TextBox
 */
TextBox::TextBox(int x, int y, uint w, uint h, std::string txt){
	base = new Rectangle(0, 0, w, h);
	base->outline(1);

	leftText = new Text(h/2, "");
	rightText = new Text(h/2, "");
	highlightText = new Text(h/2, "");
	label = new Text(h/2, txt);


	leftText->setTopPadding(h/4);
	leftText->setBottomPadding(leftText->getTopPadding());
	leftText->setX(leftText->getTopPadding());
	leftText->setRestriction(base->getOutlineThickness(), base->getOutlineThickness(),
														base->getWidth() - base->getOutlineThickness()*2,
														base->getHeight() - base->getOutlineThickness()*2);

	rightText->setTopPadding(leftText->getTopPadding());
	rightText->setBottomPadding(leftText->getBottomPadding());
	rightText->setX(leftText->getX());
	rightText->setRestriction(leftText->getRestriction());

	highlightText->setTopPadding(leftText->getTopPadding());
	highlightText->setBottomPadding(leftText->getBottomPadding());
	highlightText->setX(leftText->getX());
	highlightText->setRestriction(leftText->getRestriction());

	label->setTopPadding(leftText->getTopPadding());
	label->setBottomPadding(leftText->getBottomPadding());
	label->setX(leftText->getX());
	label->setRestriction(leftText->getRestriction());

	cursor = new Line(leftText->getX(), (h - leftText->getAscent() + leftText->getDescent()) / 2,
										leftText->getAscent() - leftText->getDescent(), baseColor[0], VERTICAL);

	fullString = "";

	cursorPos = 0;

	highlightView = new Rectangle(highlightText->getX(), cursor->getY(), 0, cursor->getLength());

	leftGradient = new Icon(leftText->getX(), base->getHeight() - base->getOutlineThickness()*2);
	leftGradient->setX(base->getOutlineThickness());
	leftGradient->setY(base->getOutlineThickness());

	rightGradient = new Icon(leftText->getX(), base->getHeight() - base->getOutlineThickness()*2);
	rightGradient->setX(base->getWidth() - base->getOutlineThickness() - leftText->getX());
	rightGradient->setY(base->getOutlineThickness());

	Color lineColor = Color(0xFFFFFFFF);
	uchar alpha = lineColor.getAlpha();
	for(int i=0;i<leftGradient->getWidth();i++){
		for(int j=0;j<leftGradient->getHeight();j++){
			leftGradient->setPixel(i, j, lineColor);
		}

		lineColor.setAlpha(lineColor.getAlpha() - alpha/leftGradient->getWidth());
	}

	for(int x=0;x<leftGradient->getWidth();x++){
		for(int y=0;y<leftGradient->getHeight();y++){
			rightGradient->setPixel(rightGradient->getWidth() - x - 1, y, leftGradient->getPixel(x, y));
		}
	}


	setX(x);
	setY(y);

	updateTheme();
}


/*get the width*/
int TextBox::getWidth(){
	return base->getWidth();
}

/*get the height*/
int TextBox::getHeight(){
	return base->getHeight();
}

/*get the total width*/
int TextBox::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int TextBox::getTotalHeight(){
	return getHeight();
}

/*is the control active?*/
bool TextBox::getActive(){
	return active;
}

/*get the text in the TextBox*/
std::string TextBox::getText(){
	return fullString;
}


/*set the X position*/
bool TextBox::setX(double x){

	base->setOffsetX(offsetX + x);
	highlightView->setOffsetX(offsetX + x);
	leftText->setOffsetX(offsetX + x);
	rightText->setOffsetX(offsetX + x);
	highlightText->setOffsetX(offsetX + x);
	label->setOffsetX(offsetX + x);
	cursor->setOffsetX(offsetX + x);
	leftGradient->setOffsetX(offsetX + x);
	rightGradient->setOffsetX(offsetX + x);


	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool TextBox::setY(double y){

	base->setOffsetY(offsetY + y);
	highlightView->setOffsetY(offsetY + y);
	leftText->setOffsetY(offsetY + y);
	rightText->setOffsetY(offsetY + y);
	highlightText->setOffsetY(offsetY + y);
	label->setOffsetY(offsetY + y);
	cursor->setOffsetY(offsetY + y);
	leftGradient->setOffsetY(offsetY + y);
	rightGradient->setOffsetY(offsetY + y);
	

	yPos = y;

	return yPos == y;
}

/*
	set the width
	this function always fails
*/
bool TextBox::setWidth(uint w){
	return false;
}

/*
	set the height
	this function always fails
*/
bool TextBox::setHeight(uint h){
	return false;
}

/*set if the control is active*/
bool TextBox::setActive(bool b){
	active = b;

	if(b){
		textureLock = false;

		if(focus){
			focus = false;
		}else{
			startTime = SDL_GetTicks();
		}
	}else if(!focus){
		if(cursorPos == leftText->getString().length()){
			rightText->setString(highlightText->getString() + rightText->getString());
			highlightText->setString("");
		}else{
			leftText->setString(leftText->getString() + highlightText->getString());
			highlightText->setString("");
		}

		textureLock = true;
		redraw = true;
	}

	return active == b;
}

/*set if the control is in focus*/
bool TextBox::setFocus(bool b){
	focus = b;

	if(b){
		textureLock = false;

		if(active){
			setActive(false);
		}else{
			startTime = SDL_GetTicks();
		}
	}else if(!active){
		textureLock = true;
		redraw = true;
	}

	return focus == b;
}

/*set the text*/
bool TextBox::setText(std::string s){
	fullString = s;

	leftText->setString("");
	rightText->setString(s);
	highlightText->setString("");

	update();

	return true;
}


/*try to handle the given event*/
bool TextBox::handleEvent(Event *event){
	int adjustedX = event->getX() - offsetX - xPos;

	if(event->getType() == e_CLICK){

		if(event->getButton() == b_LEFT && boundsCheck(event->getX(), event->getY())){
			if(event->getClick() == c_SINGLE){

				if(!active){
					setActive(true);

					cursorPos = fullString.length();

					leftText->setString("");
					rightText->setString("");
					highlightText->setString(fullString);
				}

				update();
				event->setControl(this);
			}else if(event->getClick() == c_DOUBLE){

				cursorPos = fullString.length();

				leftText->setString("");
				rightText->setString("");
				highlightText->setString(fullString);

				update();
				event->setControl(this);
			}
		}

	}else if(event->getType() == e_KEYPRESS && event->getKeyState() == s_DOWN && active){
		if(event->getKey() == SDLK_c && event->getModifier() == m_CONTROL){

			if(highlightText->getString() != ""){
				SDL_SetClipboardText(highlightText->getString().c_str());
			}

			update();
			event->setControl(this);

		}else if(event->getKey() == SDLK_v && event->getModifier() == m_CONTROL){

			std::string clip = SDL_GetClipboardText();
			leftText->setString(leftText->getString() + clip);
			cursorPos = leftText->getString().length();
			highlightText->setString("");

			update();
			event->setControl(this);

		}else if(event->getKey() == SDLK_BACKSPACE){

			if(highlightText->getString() != ""){
				cursorPos = leftText->getString().length();
				highlightText->setString("");
			}else if(leftText->getString().length() > 0){
				leftText->setString(leftText->getString().substr(0, leftText->getString().length()-1));
				cursorPos = leftText->getString().length();
			}

			fullString = leftText->getString() + highlightText->getString() + rightText->getString();

			update();
			event->setControl(this);

		}else if(event->getKey() == SDLK_LEFT){

			if(event->getModifier() == m_SHIFT){

				if(cursorPos > 0){
					
					if(cursorPos == leftText->getString().length()){
						std::string leftString = leftText->getString();

						highlightText->setString(leftString[leftString.length()-1] + highlightText->getString());
						leftText->setString(leftString.substr(0, leftString.length()-1));
						cursorPos = leftText->getString().length();
						
					}else{
						std::string highlightString = highlightText->getString();

						highlightText->setString(highlightString.substr(0, highlightString.length()-1));
						rightText->setString(highlightString[highlightString.length()-1] + rightText->getString());
						cursorPos = leftText->getString().length() + highlightText->getString().length();

					}
				}

			}else{

				if(highlightText->getString() != ""){
					rightText->setString(highlightText->getString() + rightText->getString());
					highlightText->setString("");
					cursorPos = leftText->getString().length();

				}else if(cursorPos > 0){

					std::string leftString = leftText->getString();

					rightText->setString(leftString[leftString.length()-1] + rightText->getString());
					leftText->setString(leftString.substr(0, leftString.length()-1));
					cursorPos = leftText->getString().length();

				}

			}

			update();
			event->setControl(this);

		}else if(event->getKey() == SDLK_RIGHT){

			if(event->getModifier() == m_SHIFT){

				if(cursorPos < fullString.length()){
					if(cursorPos == leftText->getString().length() + highlightText->getString().length()){
						std::string rightString = rightText->getString();

						highlightText->setString(highlightText->getString() + rightString[0]);
						rightText->setString(rightString.substr(1));
						cursorPos = leftText->getString().length() + highlightText->getString().length();
					}else{
						std::string highlightString = highlightText->getString();

						leftText->setString(leftText->getString() + highlightString[0]);
						highlightText->setString(highlightString.substr(1));
						cursorPos = leftText->getString().length();
					}


				}

			}else{

				if(highlightText->getString() != ""){
					leftText->setString(leftText->getString() + highlightText->getString());
					highlightText->setString("");
					cursorPos = leftText->getString().length();

				}else if(cursorPos < fullString.length()){
					std::string rightString = rightText->getString();

					leftText->setString(leftText->getString() + rightString[0]);
					rightText->setString(rightString.substr(1));
					cursorPos = leftText->getString().length();

				}

			}

			update();
			event->setControl(this);

		}else if(event->getKey() == SDLK_UP){

			cursorPos = 0;

			leftText->setString("");
			rightText->setString(fullString);
			highlightText->setString("");

			update();
			event->setControl(this);

		}else if(event->getKey() == SDLK_DOWN){

			cursorPos = fullString.length();

			leftText->setString(fullString);
			rightText->setString("");
			highlightText->setString("");

			update();
			event->setControl(this);

		}


	}else if(event->getType() == e_TEXTENTRY && active){

		highlightText->setString("");
		leftText->setString(leftText->getString() + event->getText());
		cursorPos = leftText->getString().length();

		fullString = leftText->getString() + highlightText->getString() + rightText->getString();

		update();
		event->setControl(this);

	}else if(event->getType() == e_MOUSEPRESS && event->getMouseState() == s_DOWN){

		if(boundsCheck(event->getX(), event->getY())){

			if(!active){
				setFocus(true);
			}

			


			if(leftText->boundsCheck(event->getX(), leftText->getY() + leftText->getOffsetY())){

				double averageWidth = (double)leftText->getWidth() / (double)leftText->getString().length();

				int pos = nearbyint((double)(adjustedX - leftText->getX()) / averageWidth);

				highlightText->setString("");

				leftText->setString(fullString.substr(0, pos));
				rightText->setString(fullString.substr(pos));

				cursorPos = leftText->getString().length();

			}else if(rightText->boundsCheck(event->getX(), rightText->getY() + rightText->getOffsetY())){
				double averageWidth = (double)rightText->getWidth() / (double)rightText->getString().length();

				int pos = nearbyint((double)(adjustedX - rightText->getX()) / averageWidth);
				pos += leftText->getString().length() + highlightText->getString().length();

				highlightText->setString("");

				leftText->setString(fullString.substr(0, pos));
				rightText->setString(fullString.substr(pos));

				cursorPos = leftText->getString().length();

			}else if(highlightText->boundsCheck(event->getX(), highlightText->getY() + highlightText->getOffsetY())){
				double averageWidth = (double)highlightText->getWidth() / (double)highlightText->getString().length();

				int pos = nearbyint((double)(adjustedX - highlightText->getX()) / averageWidth);
				pos += leftText->getString().length();

				highlightText->setString("");

				leftText->setString(fullString.substr(0, pos));
				rightText->setString(fullString.substr(pos));

				cursorPos = leftText->getString().length();

			}else if(adjustedX < leftText->getX()){
				cursorPos = 0;

				rightText->setString(fullString);
				leftText->setString("");
				highlightText->setString("");

			}else if(adjustedX > rightText->getX() + rightText->getWidth()){
				cursorPos = fullString.length();

				leftText->setString(fullString);
				rightText->setString("");
				highlightText->setString("");

			}




			update();
			event->setControl(this);
		}else if(!boundsCheck(event->getX(), event->getY()) && active){

			setActive(false);
			update();

		}

	}else if(event->getType() == e_DRAG && (active || focus)){

		if(boundsCheck(event->getX(), yPos + offsetY)){

			if(!active){
				setActive(true);
			}

			


			if(leftText->boundsCheck(event->getX(), leftText->getY() + leftText->getOffsetY())){

				double averageWidth = (double)leftText->getWidth() / (double)leftText->getString().length();

				int pos = nearbyint((double)(adjustedX - leftText->getX()) / averageWidth);

				if(cursorPos == leftText->getString().length()){
					highlightText->setString(leftText->getString().substr(pos) + highlightText->getString());
					leftText->setString(leftText->getString().substr(0, pos));

					cursorPos = leftText->getString().length();
				}else{
					rightText->setString(highlightText->getString() + rightText->getString());
					highlightText->setString(leftText->getString().substr(pos));
					leftText->setString(leftText->getString().substr(0, pos));

					cursorPos = leftText->getString().length();
				}

			}else if(rightText->boundsCheck(event->getX(), rightText->getY() + rightText->getOffsetY())){
				double averageWidth = (double)rightText->getWidth() / (double)rightText->getString().length();

				int pos = nearbyint((double)(adjustedX - rightText->getX()) / averageWidth);

				if(cursorPos == leftText->getString().length() + highlightText->getString().length()){
					highlightText->setString(highlightText->getString() + rightText->getString().substr(0, pos));
					rightText->setString(rightText->getString().substr(pos));

					cursorPos = leftText->getString().length() + highlightText->getString().length();
				}else{
					leftText->setString(leftText->getString() + highlightText->getString());
					highlightText->setString(rightText->getString().substr(0, pos));
					rightText->setString(rightText->getString().substr(pos));

					cursorPos = leftText->getString().length() + highlightText->getString().length();
				}

			}else if(highlightText->boundsCheck(event->getX(), highlightText->getY() + highlightText->getOffsetY())){
				double averageWidth = (double)highlightText->getWidth() / (double)highlightText->getString().length();

				int pos = nearbyint((double)(adjustedX - highlightText->getX()) / averageWidth);

				if(cursorPos == leftText->getString().length()){
					leftText->setString(leftText->getString() + highlightText->getString().substr(0, pos));
					highlightText->setString(highlightText->getString().substr(pos));

					cursorPos = leftText->getString().length();
				}else{
					rightText->setString(highlightText->getString().substr(pos) + rightText->getString());
					highlightText->setString(highlightText->getString().substr(0, pos));

					cursorPos = leftText->getString().length() + highlightText->getString().length();
				}

			}





		}else if(adjustedX < base->getX() && cursorPos != 0 && (SDL_GetTicks() - startTime) % 75 == 0){

			cursorPos -= 1;

			if(cursorPos+1 == leftText->getString().length()){
				highlightText->setString(leftText->getString().substr(cursorPos) + highlightText->getString());
				leftText->setString(leftText->getString().substr(0, cursorPos));
			}else{
				rightText->setString(highlightText->getString().substr(cursorPos - leftText->getString().length()) + rightText->getString());
				highlightText->setString(highlightText->getString().substr(0, cursorPos - leftText->getString().length()));
			}

		}else if(adjustedX > base->getX() + base->getWidth() && cursorPos != fullString.length() && (SDL_GetTicks() - startTime) % 75 == 0){

			cursorPos += 1;

			if(cursorPos-1 == leftText->getString().length()){
				leftText->setString(leftText->getString() + highlightText->getString().substr(0, 1));
				highlightText->setString(highlightText->getString().substr(1));
			}else{
				highlightText->setString(highlightText->getString() + rightText->getString().substr(0, 1));
				rightText->setString(rightText->getString().substr(1));
			}

		}

		update();
		event->setControl(this);

	}else if(event->getType() == e_HOLD && active && !boundsCheck(event->getX(), event->getY())){

		if(adjustedX < base->getX() && cursorPos != 0 && (SDL_GetTicks() - startTime) % 75 == 0){

			cursorPos -= 1;

			if(cursorPos+1 == leftText->getString().length()){
				highlightText->setString(leftText->getString().substr(cursorPos) + highlightText->getString());
				leftText->setString(leftText->getString().substr(0, cursorPos));
			}else{
				rightText->setString(highlightText->getString().substr(cursorPos - leftText->getString().length()) + rightText->getString());
				highlightText->setString(highlightText->getString().substr(0, cursorPos - leftText->getString().length()));
			}

		}else if(adjustedX > base->getX() + base->getWidth() && cursorPos != fullString.length() && (SDL_GetTicks() - startTime) % 75 == 0){

			cursorPos += 1;

			if(cursorPos-1 == leftText->getString().length()){
				leftText->setString(leftText->getString() + highlightText->getString().substr(0, 1));
				highlightText->setString(highlightText->getString().substr(1));
			}else{
				highlightText->setString(highlightText->getString() + rightText->getString().substr(0, 1));
				rightText->setString(rightText->getString().substr(1));
			}

		}

		update();
		event->setControl(this);
	}else if(event->getType() == e_MOVE){

		if(boundsCheck(event->getX(), event->getY())){
			setCursor(SDL_SYSTEM_CURSOR_IBEAM);
		}else if(boundsCheck(event->getPreviousX(), event->getPreviousY())){
			setCursor(SDL_SYSTEM_CURSOR_ARROW);
		}

	}

}


/*update the theme of the control*/
bool TextBox::updateTheme(){

	//darkmode is set
	if(darkMode){

		//the base is the wrong color
		if(base->getFillColor().getUint() != darkTheme[1]->getUint()){
			leftText->setColor(*darkTheme[2]);
			highlightText->setColor(*darkTheme[1]);
			rightText->setColor(*darkTheme[2]);

			leftGradient->setColor(*darkTheme[1]);
			rightGradient->setColor(*darkTheme[1]);

			label->setColor(*darkTheme[3]);
		}

		base->setFillColor(*darkTheme[1]);

		if(active || focus){
			base->setOutlineColor(*accent);
		}else{
			base->setOutlineColor(*darkTheme[2]);
		}

		highlightView->setFillColor(*accent);
		cursor->setColor(*accent);

	}else{

		if(base->getFillColor().getUint() != lightTheme[1]->getUint()){
			leftText->setColor(*lightTheme[2]);
			highlightText->setColor(*lightTheme[1]);
			rightText->setColor(*lightTheme[2]);

			leftGradient->setColor(*lightTheme[1]);
			rightGradient->setColor(*lightTheme[1]);

			label->setColor(*lightTheme[3]);
		}

		base->setFillColor(*lightTheme[1]);

		if(active || focus){
			base->setOutlineColor(*accent);
		}else{
			base->setOutlineColor(*lightTheme[2]);
		}

		highlightView->setFillColor(*accent);
		cursor->setColor(*accent);

	}

	redraw = true;
}

/*free all the memory used*/
void TextBox::free(){
	delete leftText->getRestriction();

	base->free();
	highlightView->free();
	leftText->free();
	rightText->free();
	highlightText->free();

	label->free();

	cursor->free();

	leftGradient->free();
	rightGradient->free();


	delete base;
	delete highlightView;
	delete leftText;
	delete rightText;
	delete highlightText;

	delete label;

	delete cursor;

	delete leftGradient;
	delete rightGradient;


	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the TextBox*/
bool TextBox::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*
	update anything that needs updated
	the control decides when to update itself
	this function does nothing
*/
void TextBox::update(){

	fullString = leftText->getString() + highlightText->getString() + rightText->getString();

	highlightText->setX(leftText->getX() + leftText->getWidth());

	if(cursorPos == leftText->getString().length()){
		cursor->setX(leftText->getX() + leftText->getWidth());
		
		if(cursor->getX() > rightGradient->getX()){
			cursor->setX(rightGradient->getX());
			leftText->setX(cursor->getX() - leftText->getWidth());
		}else if(cursor->getX() < leftGradient->getX() + leftGradient->getWidth()){
			cursor->setX(leftGradient->getX() + leftGradient->getWidth());
			leftText->setX(cursor->getX() - leftText->getWidth());
		}
	}else{
		cursor->setX(highlightText->getX() + highlightText->getWidth());
		
		if(cursor->getX() > rightGradient->getX()){
			cursor->setX(rightGradient->getX());
			leftText->setX(cursor->getX() - highlightText->getWidth() - leftText->getWidth());
		}else if(cursor->getX() < leftGradient->getX() + leftGradient->getWidth()){
			cursor->setX(leftGradient->getX() + leftGradient->getWidth());
			leftText->setX(cursor->getX() - highlightText->getWidth() - leftText->getWidth());
		}
	}

	highlightText->setX(leftText->getX() + leftText->getWidth());
	rightText->setX(highlightText->getX() + highlightText->getWidth());
	highlightView->setX(highlightText->getX());
	highlightView->setWidth(highlightText->getWidth());


	if(active || focus){
		base->setOutlineColor(*accent);
	}else if(darkMode){
		base->setOutlineColor(*darkTheme[2]);
	}else{
		base->setOutlineColor(*lightTheme[2]);
	}

	redraw = true;
}

/*update the texture of the TextBox to be printed later*/
bool TextBox::updateTexture(SDL_Renderer *renderer){

	base->draw(renderer);

	if(fullString == ""){
		label->draw(renderer);
	}

	if(leftText->getString() != ""){
		leftText->draw(renderer);
	}

	if(highlightText->getString() != ""){
		highlightView->draw(renderer);
		highlightText->draw(renderer);
	}

	if(rightText->getString() != ""){
		rightText->draw(renderer);
	}

	leftGradient->draw(renderer);
	rightGradient->draw(renderer);

	if((active || focus) && (SDL_GetTicks() - startTime) / 500 % 2 == 0){
		cursor->draw(renderer);
	}

}
