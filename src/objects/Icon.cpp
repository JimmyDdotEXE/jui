#include "objects/Icon.h"


/*
	Icon constructor

	std::string fileName: image file name to load
 */
Icon::Icon(std::string fileName) : Image(0, 0, fileName){}

/*
	Icon constructor

	uint w: width of the icon
	uint h: height of the icon
	Color color: fill color of the icon
 */
Icon::Icon(uint w, uint h, Color color) : Image(0, 0, w, h, color){}


/*sets the color value of all non-transparent pixels to the new color value*/
bool Icon::setColor(Color color){
	for(int i=0;i < width;i++){
		for(int j=0;j < height;j++){
			Color c = getPixel(i, j);

			if(c.getAlpha() != 0){
				color.setAlpha(c.getAlpha());

				setPixel(i, j, color);
			}
		}
	}

	redraw = true;
	return true;
}


/*replaces all pizels with a given color value to a new one*/
bool Icon::replaceColor(Color from, Color to){
	for(int i=0;i < width;i++){
		for(int j=0;j < height;j++){
			Color c = getPixel(i, j);

			if(c.getUint() >> 2 == from.getUint() >> 2){
				to.setAlpha(c.getAlpha());

				setPixel(i, j, to);
			}
		}
	}

	redraw = true;
	return true;
}
