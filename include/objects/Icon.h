#ifndef ICON_H
#define ICON_H

#include "objects/Image.h"


class Icon : public Image{
	public:
		Icon(std::string fileName);
		Icon(uint w, uint h, Color color=Color());

		bool setColor(Color color);

		bool replaceColor(Color from, Color to);
};

#endif
