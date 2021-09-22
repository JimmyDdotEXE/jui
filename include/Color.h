#ifndef COLOR_H
#define COLOR_H

#include "General.h"


class Color{
	public:
		Color(uchar r, uchar g, uchar b, uchar a=0xFF);
		Color(uint color=0x00000000);

		uchar getRed();
		uchar getGreen();
		uchar getBlue();
		uchar getAlpha();
		uint getUint();

		void setRed(uchar r);
		void setGreen(uchar g);
		void setBlue(uchar b);
		void setAlpha(uchar a);
		void setUint(uint color);

		void invert();


	private:
		uchar red;
		uchar green;
		uchar blue;
		uchar alpha;
};

#endif
