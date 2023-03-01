#include <iostream>
#include "objects/Text.h"


/*
	Text constructor

	uint size: font size
	std::string s: string for the Text
	Color color: text color
	bool wspace: does font size take whitespace into account
 */
Text::Text(uint size, std::string s, Color c, bool wSpace){
	text = NULL;
	font = NULL;
	
	whitespace = wSpace;

	topPadding = 0;
	bottomPadding = 0;
	leftPadding = 0;
	rightPadding = 0;

	if(load(size, c, s)){
		setX(0);
		setY(0);
	}
}

/*
	Text constructor

	int x: X position of the Text
	int y: Y position of the Text
	uint size: font size
	std::string s: string for the Text
	Color color: text color
	bool wspace: does font size take whitespace into account
 */
Text::Text(int x, int y, uint size, std::string s, Color c, bool wSpace){
	text = NULL;
	font = NULL;
	
	whitespace = wSpace;

	topPadding = 0;
	bottomPadding = 0;
	leftPadding = 0;
	rightPadding = 0;

	if(load(size, c, s)){
		setX(x);
		setY(y);
	}
}


/*get the width*/
int Text::getWidth(){
	if(text && str != ""){
		return leftPadding + text->w + rightPadding;
	}else{
		return leftPadding + rightPadding;
	}
}

/*get the height*/
int Text::getHeight(){
	if(bottomPadding > -1*getDescent()){
		return topPadding + getAscent() + bottomPadding;
	}else{
		return topPadding + getAscent() + -1*getDescent();
	}
}

/*get the total width*/
int Text::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int Text::getTotalHeight(){
	return getHeight();
}

/*get the font size*/
int Text::getFontSize(){
	return fontSize;
}

/*tells if the font size takes the top white space into acount*/
bool Text::getWhitespace(){
	return whitespace;
}

/*get the string of the text*/
std::string Text::getString(){
	return str;
}

/*get the text color*/
Color Text::getColor(){
	return color;
}

/*get the padding above the Text*/
int Text::getTopPadding(){
	return topPadding;
}

/*get the padding bellow the Text*/
int Text::getBottomPadding(){
	return bottomPadding;
}

/*get the padding to the left of the Text*/
int Text::getLeftPadding(){
	return leftPadding;
}

/*get the padding to the right of the Text*/
int Text::getRightPadding(){
	return rightPadding;
}

/*
	get the ascent of the font
	ascent is the amount of the font above the baseline
*/
int Text::getAscent(){
	if(font == NULL){
		return 0;
	}else if(whitespace){
		return TTF_FontAscent(font);
	}else{
		return TTF_FontAscent(font) + TTF_FontDescent(font);
	}
}

/*
	get the descent of the font
	descent is the amount of the font below the baseline
*/
int Text::getDescent(){
	if(font == NULL){
		return 0;
	}else{
		return TTF_FontDescent(font);
	}
}


/*set the X position*/
bool Text::setX(double x){
	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool Text::setY(double y){
	yPos = y;

	return yPos == y;
}

/*set the width*/
bool Text::setWidth(uint w){
	return false;
}

/*set the height*/
bool Text::setHeight(uint h){
	return false;
}

/*set the size of the font*/
bool Text::setFontSize(uint size){
	return load(size, color, str);
}

/*set if the font size takes the top white space into acount*/
bool Text::setWhitespace(bool w){
	whitespace = w;

	return load(fontSize, color, str);
}

/*set the string of the text*/
bool Text::setString(std::string s){
	str = s;
	
	if(s == ""){
		s = " ";
	}

	SDL_FreeSurface(text);
	text = NULL;

	redraw = true;

	if(font != NULL){
		if(text = TTF_RenderText_Blended(font, s.c_str(), {color.getRed(), color.getGreen(), color.getBlue()})){

			return true;
		}else{
			return false;
		}
	}else{
		return load(fontSize, color, s);
	}
}

/*set the text color*/
bool Text::setColor(Color c){
	return load(fontSize, c, str);
}

/*set left, right, top, and bottom padding to the same value*/
bool Text::setPadding(uint pad){
	topPadding = pad;
	bottomPadding = pad;
	leftPadding = pad;
	rightPadding = pad;

	return true;
}

/*set the padding above the Text*/
bool Text::setTopPadding(uint pad){
	topPadding = pad;
	return topPadding == pad;
}

/*set the padding bellow the Text*/
bool Text::setBottomPadding(uint pad){
	bottomPadding = pad;
	return bottomPadding == pad;
}

/*set the padding to the left of the Text*/
bool Text::setLeftPadding(uint pad){
	leftPadding = pad;
	return leftPadding == pad;
}

/*set the padding to the right of the Text*/
bool Text::setRightPadding(uint pad){
	rightPadding = pad;
	return rightPadding == pad;
}


/*load the text and font using the given font size, color, and string*/
bool Text::load(uint size, Color c, std::string s){
	str = s;
	
	if(s == ""){
		s = " ";
	}

	for(int i=size;;i++){
		free();
		font = TTF_OpenFont(fontLocation.c_str(), i);

		if(font == NULL){
			break;
		}else if(getAscent() > size){
			free();
			font = TTF_OpenFont(fontLocation.c_str(), i-1);
			break;
		}else if(whitespace){
			break;
		}
	}


	if(font){
		if(text = TTF_RenderText_Blended(font, s.c_str(), {c.getRed(), c.getGreen(), c.getBlue()})){
			fontSize = size;
			color = c;

			return true;
		}else{
			return false;
		}
	}else{
		std::cout << TTF_GetError() << std::endl;

		return false;
	}
}


/*free all the memory used*/
void Text::free(){
	SDL_FreeSurface(text);
	TTF_CloseFont(font);

	text = NULL;
	font = NULL;

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the text*/
bool Text::boundsCheck(int x, int y){
	x -= getOffsetX();
	y -= getOffsetY();

	return x >= xPos && x < xPos + getWidth() && y >= yPos && y < yPos + getHeight();
}




/*update the texture of the text to be printed later*/
bool Text::updateTexture(SDL_Renderer *renderer){

	if(renderer && text){
		SDL_Texture *temp = SDL_CreateTextureFromSurface(renderer, text);
		SDL_SetTextureBlendMode(temp, SDL_BLENDMODE_NONE);

		SDL_Rect dest;

		dest.x = leftPadding;

		if(whitespace){
			dest.y = topPadding;
		}else{
			dest.y = getDescent() + topPadding;
		}

	 	dest.w = text->w;
		dest.h = text->h;

		SDL_RenderCopy(renderer, temp, NULL, &dest);

		SDL_DestroyTexture(temp);
		return true;
	}

	return false;
}
