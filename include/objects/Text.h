#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

#include "Color.h"
#include "objects/Rectangle.h"
#include "objects/DrawObject.h"

class Text : public DrawObject{
	public:
		Text(uint size, std::string s, Color c=Color(), bool wSpace=false);
		Text(int x, int y, uint size, std::string s, Color c=Color(), bool wSpace=false);

		virtual int getWidth();
		virtual int getHeight();
		virtual int getTotalWidth();
		virtual int getTotalHeight();
		int getFontSize();
		bool getWhitespace();
		std::string getString();
		Color getColor();
		int getTopPadding();
		int getBottomPadding();
		int getLeftPadding();
		int getRightPadding();
		int getAscent();
		int getDescent();

		virtual bool setX(double x);
		virtual bool setY(double Y);
		virtual bool setWidth(uint w);
		virtual bool setHeight(uint h);
		bool setFontSize(uint size);
		bool setWhitespace(bool w);
		bool setString(std::string s);
		bool setColor(Color c);
		bool setTopPadding(uint pad);
		bool setBottomPadding(uint pad);
		bool setLeftPadding(uint pad);
		bool setRightPadding(uint pad);
		bool setPadding(uint pad);

		bool load(uint size, Color c, std::string s);

		virtual void free();
		virtual bool boundsCheck(int x, int y);

	protected:
		virtual bool updateTexture(SDL_Renderer *renderer);


		SDL_Surface *text;
		TTF_Font *font;
		std::string str;
		uint fontSize;
		uint actualSize;
		Color color;

		bool whitespace;
		uint topPadding;
		uint bottomPadding;
		uint leftPadding;
		uint rightPadding;
};

#endif
