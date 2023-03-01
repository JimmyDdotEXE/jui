#include <iostream>
#include <algorithm>
#include "controls/View.h"


/*
	View constructor

	int x: X position of the View
	int y: Y position of the View
	uint w: width of the View
	uint h: height of the View
 */
View::View(int x, int y, uint w, uint h){
	width = w;
	height = h;

	viewX = 0;
	viewY = 0;
	viewWidth = w;
	viewHeight = h;

	view = NULL;

	clearColor = Color();

	verticalLock = false;
	horizontalLock = false;

	verticalBar = NULL;
	horizontalBar = NULL;

	scrollCorner = new Rectangle(width-16, height-16, 16, 16);

	setX(x);
	setY(y);

	textureLock = false;

	updateTheme();
}


/*get the width*/
int View::getWidth(){
	return width;
}

/*get the height*/
int View::getHeight(){
	return height;
}

/*get the total width*/
int View::getTotalWidth(){
	return getWidth();
}

/*get the total height*/
int View::getTotalHeight(){
	return getHeight();
}


/*set the X position*/
bool View::setX(double x){
	for(int i=0;i<drawQueue.size();i++){
		drawQueue.at(i)->setOffsetX(offsetX + x);
	}

	for(int i=0;i<controls.size();i++){
		controls.at(i)->setOffsetX(offsetX + x);
	}

	if(verticalBar){
		verticalBar->setOffsetX(offsetX + x);
	}

	if(horizontalBar){
		horizontalBar->setOffsetX(offsetX + x);
	}

	xPos = x;

	return xPos == x;
}

/*set the Y position*/
bool View::setY(double y){

	for(int i=0;i<drawQueue.size();i++){
		drawQueue.at(i)->setOffsetY(offsetY + y);
	}

	for(int i=0;i<controls.size();i++){
		controls.at(i)->setOffsetY(offsetY + y);
	}

	if(verticalBar){
		verticalBar->setOffsetY(offsetY + y);
	}

	if(horizontalBar){
		horizontalBar->setOffsetY(offsetY + y);
	}

	yPos = y;

	return yPos == y;
}

/*set the width*/
bool View::setWidth(uint w){
	width = w;

	if(horizontalBar != NULL && verticalBar != NULL){
		horizontalBar->setWidth(w - verticalBar->getWidth());
		verticalBar->setX(w - verticalBar->getWidth());
	}else if(horizontalBar != NULL){
		horizontalBar->setWidth(w);
	}else if(verticalBar != NULL){
		verticalBar->setX(w - verticalBar->getWidth());
	}


	if(scrollCorner != NULL){
		scrollCorner->setX(w - 16);
	}


	if(verticalBar != NULL){
		if(w - verticalBar->getWidth() > viewWidth){
			setViewWidth(w - verticalBar->getWidth());
		}else if(w - verticalBar->getWidth() < viewWidth){
			uint neededWidth = w - verticalBar->getWidth();
			uint rightSpace = 0;


			for(int i=0;i<controls.size();i++){
				Control *control = controls.at(i);

				if(control->getX() + control->getWidth() > neededWidth && !control->getRightLock()){
					neededWidth = control->getX() + control->getWidth();
				}else if(control->getRightLock() && !control->getLeftLock()){
					uint temp = viewWidth - control->getX();

					if(temp > rightSpace){
						rightSpace = temp;
					}
				}
			}

			for(int i=0;i<drawQueue.size();i++){
				if(drawQueue.at(i)->getX() + drawQueue.at(i)->getWidth() > neededWidth){
					neededWidth = drawQueue.at(i)->getX() + drawQueue.at(i)->getWidth();
				}
			}


			if(!horizontalLock){
				setViewWidth(neededWidth + rightSpace);
			}
		}
	}else if(w > viewWidth){
		setViewWidth(w);
	}else if(w < viewWidth){
		uint neededWidth = w;
		uint rightSpace = 0;


		for(int i=0;i<controls.size();i++){
			Control *control = controls.at(i);

			if(control->getX() + control->getWidth() > neededWidth && !controls.at(i)->getRightLock()){
				neededWidth = control->getX() + control->getWidth();
			}else if(control->getRightLock() && !control->getLeftLock()){
				uint temp = viewWidth - control->getX();

				if(temp > rightSpace){
					rightSpace = temp;
				}
			}
		}

		for(int i=0;i<drawQueue.size();i++){
			DrawObject *object = drawQueue.at(i);

			if(object->getX() + object->getWidth() > neededWidth){
				neededWidth = object->getX() + object->getWidth();
			}
		}


		if(!horizontalLock){
			setViewWidth(neededWidth + rightSpace);
		}
	}


	update();

	return width == w;
}

/*set the height*/
bool View::setHeight(uint h){
	height = h;

	if(horizontalBar != NULL && verticalBar != NULL){
		verticalBar->setHeight(h - horizontalBar->getHeight());
		horizontalBar->setY(h - horizontalBar->getHeight());
	}else if(verticalBar != NULL){
		verticalBar->setHeight(h);
	}else if(horizontalBar != NULL){
		horizontalBar->setY(h - horizontalBar->getHeight());
	}


	if(scrollCorner != NULL){
		scrollCorner->setY(h - 16);
	}


	if(horizontalBar != NULL){
		if(h - horizontalBar->getHeight() > viewHeight){
			setViewHeight(h - horizontalBar->getHeight());
		}else if(h - horizontalBar->getHeight() < viewHeight){
			uint neededHeight = h - horizontalBar->getHeight();
			uint bottomSpace = 0;

			for(int i=0;i<controls.size();i++){
				Control *control = controls.at(i);

				if(control->getY() + control->getHeight() > neededHeight && !control->getBottomLock()){
					neededHeight = control->getY() + control->getHeight();
				}else if(control->getBottomLock() && !control->getTopLock()){
					uint temp = viewHeight - control->getY();

					if(temp > bottomSpace){
						bottomSpace = temp;
					}
				}
			}

			for(int i=0;i<drawQueue.size();i++){
				if(drawQueue.at(i)->getY() + drawQueue.at(i)->getHeight() > neededHeight){
					neededHeight = drawQueue.at(i)->getY() + drawQueue.at(i)->getHeight();
				}
			}

			if(!verticalLock){
				setViewHeight(neededHeight + bottomSpace);
			}
		}
	}else if(h > viewHeight){
		setViewHeight(h);
	}else if(h < viewHeight){
		uint neededHeight = h;
		uint bottomSpace = 0;

		for(int i=0;i<controls.size();i++){
			Control *control = controls.at(i);

			if(control->getY() + control->getHeight() > neededHeight && !control->getBottomLock()){
				neededHeight = control->getY() + control->getHeight();
			}else if(control->getBottomLock() && !control->getTopLock()){
				uint temp = viewHeight - control->getY();

				if(temp > bottomSpace){
					bottomSpace = temp;
				}
			}
		}

		for(int i=0;i<drawQueue.size();i++){
			if(drawQueue.at(i)->getY() + drawQueue.at(i)->getHeight() > neededHeight){
				neededHeight = drawQueue.at(i)->getY() + drawQueue.at(i)->getHeight();
			}
		}

		if(!verticalLock){
			setViewHeight(neededHeight + bottomSpace);
		}
	}


	update();

	return height == h;
}

/*set the width of the view*/
bool View::setViewWidth(uint w){
	bool callSetHeight = false;

	SDL_DestroyTexture(view);
	view = NULL;

	uint oldWidth = viewWidth;

	if(!horizontalLock){
		viewWidth = w;


		if(verticalBar != NULL){
			if(horizontalBar != NULL){
				if(viewWidth <= width - verticalBar->getWidth()){
					horizontalBar->free();
					delete horizontalBar;
					horizontalBar = NULL;

					verticalBar->setHeight(height);
					callSetHeight = true;

					if(verticalBar != NULL){
						if(viewWidth < width - verticalBar->getWidth()){
							viewWidth = width - verticalBar->getWidth();
						}
					}else if(viewWidth < width){
						viewWidth = width;
					}

				}else{
					horizontalBar->setTotalSize(viewWidth);
				}

			}else if(viewWidth > width - verticalBar->getWidth()){
				horizontalBar = new ScrollBar(0, 0, width - verticalBar->getWidth(), viewWidth, 12, HORIZONTAL);

				verticalBar->setHeight(height - horizontalBar->getHeight());

				horizontalBar->setY(verticalBar->getHeight());
				horizontalBar->setOffsetX(offsetX + xPos);
				horizontalBar->setOffsetY(offsetY + yPos);

			}else if(viewWidth < width - verticalBar->getWidth()){
				viewWidth = width - verticalBar->getWidth();
			}

		}else if(horizontalBar != NULL){
			if(viewWidth <= width){
				horizontalBar->free();
				delete horizontalBar;
				horizontalBar = NULL;

				callSetHeight = true;

				if(viewWidth < width){
					viewWidth = width;
				}
			}else{
				horizontalBar->setTotalSize(viewWidth);
			}
		}else if(viewWidth > width){
			horizontalBar = new ScrollBar(0, 0, width, viewWidth, 12, HORIZONTAL);

			horizontalBar->setY(height - horizontalBar->getHeight());
			horizontalBar->setOffsetX(offsetX + xPos);
			horizontalBar->setOffsetY(offsetY + yPos);

			callSetHeight = true;
		}else if(viewWidth < width){
			viewWidth = width;
		}

	}else if(verticalBar != NULL){
		viewWidth = width - verticalBar->getWidth();

		if(horizontalBar != NULL){
			horizontalBar->free();
			delete horizontalBar;
			horizontalBar = NULL;

			verticalBar->setHeight(height);
			callSetHeight = true;
		}

	}else{
		viewWidth = width;

		if(horizontalBar != NULL){
			horizontalBar->free();
			delete horizontalBar;
			horizontalBar = NULL;
		}

	}


	for(int i=0;i<controls.size();i++){
		Control *control = controls.at(i);

		if(control->getLeftLock() && control->getRightLock()){
			bool noResize = true;

			if(control->getLockResize()){
				noResize = !control->setWidth(control->getWidth() + ((int)viewWidth - (int)oldWidth));
			}

			if(noResize){
				double ratio = (control->getX() + (double)(control->getWidth()/2)) / (double)oldWidth;
				control->setX((double)viewWidth*ratio - (double)(control->getWidth()/2));
			}

		}else if(controls.at(i)->getRightLock()){
			control->setX(control->getX() + ((int)viewWidth - (int)oldWidth));
		}
	}

	if(callSetHeight){
		setHeight(height);
	}

	return viewWidth == w;
}

/*set the height of the view*/
bool View::setViewHeight(uint h){
	bool callSetWidth = false;

	SDL_DestroyTexture(view);
	view = NULL;

	uint oldHeight = viewHeight;


	if(!verticalLock){
		viewHeight = h;


		if(horizontalBar != NULL){
			if(verticalBar != NULL){
				if(viewHeight <= height - horizontalBar->getHeight()){
					verticalBar->free();
					delete verticalBar;
					verticalBar = NULL;

					horizontalBar->setWidth(width);
					callSetWidth = true;

					if(horizontalBar != NULL){
						if(viewHeight < height - horizontalBar->getHeight()){
							viewHeight = height - horizontalBar->getHeight();
						}
					}else if(viewHeight < height){
						viewHeight = height;
					}

				}else{
					verticalBar->setTotalSize(viewHeight);
				}

			}else if(viewHeight > height - horizontalBar->getHeight()){
				verticalBar = new ScrollBar(0, 0, height - horizontalBar->getHeight(), viewHeight, 12, VERTICAL);

				horizontalBar->setWidth(width - verticalBar->getWidth());

				verticalBar->setX(horizontalBar->getWidth());
				verticalBar->setOffsetX(offsetX + xPos);
				verticalBar->setOffsetY(offsetY + yPos);

			}else if(viewHeight < height - horizontalBar->getHeight()){
				viewHeight = height - horizontalBar->getHeight();
			}

		}else if(verticalBar != NULL){
			if(viewHeight <= height){
				verticalBar->free();
				delete verticalBar;
				verticalBar = NULL;

				callSetWidth = true;

				if(viewHeight < height){
					viewHeight = height;
				}
			}else{
				verticalBar->setTotalSize(viewHeight);
			}
		}else if(viewHeight > height){
			verticalBar = new ScrollBar(0, 0, height, viewHeight, 12, VERTICAL);

			verticalBar->setX(width - verticalBar->getWidth());
			verticalBar->setOffsetX(offsetX + xPos);
			verticalBar->setOffsetY(offsetY + yPos);

			callSetWidth = true;
		}else if(viewHeight < height){
			viewHeight = height;
		}

	}else if(horizontalBar != NULL){
		viewHeight = height - horizontalBar->getHeight();

		if(verticalBar != NULL){
			verticalBar->free();
			delete verticalBar;
			verticalBar = NULL;

			horizontalBar->setWidth(width);
			callSetWidth = true;
		}

	}else{
		viewHeight = height;

		if(verticalBar != NULL){
			verticalBar->free();
			delete verticalBar;
			verticalBar = NULL;
		}

	}


	for(int i=0;i<controls.size();i++){
		Control *control = controls.at(i);

		if(control->getTopLock() && control->getBottomLock()){
			bool noResize = true;

			if(control->getLockResize()){
				noResize = !control->setHeight(control->getHeight() + ((int)viewHeight - (int)oldHeight));

				if(noResize){
					double ratio = (control->getY() + (double)(control->getHeight()/2)) / (double)oldHeight;
					control->setY((double)viewHeight*ratio - (double)(control->getHeight()/2));
				}
			}
		}else if(controls.at(i)->getBottomLock()){
			control->setY(control->getY() + ((int)viewHeight - (int)oldHeight));
		}
	}


	if(callSetWidth){
		setWidth(width);
	}

	return viewHeight == h;
}

/*set if the view can scroll vertically*/
bool View::setVerticalLock(bool b){
	verticalLock = b;

	return verticalLock == b;
}

/*set if the view can scroll horizontally*/
bool View::setHorizontalLock(bool b){
	horizontalLock = b;

	return horizontalLock == b;
}


/*clear the view with the given color*/
bool View::clear(Color color){
	clearColor = color;

	drawQueue.clear();


	uint vWidth = 0;
	uint vHeight = 0;
	uint rightSpace = 0;
	uint bottomSpace = 0;

	for(int i=0;i<controls.size();i++){
		Control *control = controls.at(i);

		if(control->getX() + control->getWidth() > vWidth && !control->getRightLock()){
			vWidth = control->getX() + control->getWidth();
		}else if(control->getRightLock() && !control->getLeftLock()){
			uint temp = viewWidth - control->getX();

			if(temp > rightSpace){
				rightSpace = temp;
			}
		}


		if(control->getY() + control->getHeight() > vHeight && !control->getBottomLock()){
			vHeight = control->getY() + control->getHeight();
		}else if(control->getBottomLock() && !control->getTopLock()){
			uint temp = viewHeight - control->getY();

			if(temp > bottomSpace){
				bottomSpace = temp;
			}
		}
	}

	if(vWidth + rightSpace != viewWidth){
		setViewWidth(vWidth + rightSpace);
	}

	if(vHeight + bottomSpace != viewHeight){
		setViewHeight(vHeight + bottomSpace);
	}

	redraw = true;

	return true;
}


/*add the given object to the draw queue*/
bool View::drawObject(DrawObject *object){
	if(std::find(drawQueue.begin(), drawQueue.end(), object) != drawQueue.end()){
		return false;
	}

	if(horizontalBar != NULL){
		object->setOffsetX(getOffsetX() - horizontalBar->getPos() + xPos);
	}else{
		object->setOffsetX(getOffsetX() + xPos);
	}

	if(verticalBar != NULL){
		object->setOffsetY(getOffsetY() - verticalBar->getPos() + yPos);
	}else{
		object->setOffsetY(getOffsetY() + yPos);
	}


	if(object->getX() + object->getWidth() > viewWidth){
		setViewWidth(object->getX() + object->getWidth());
	}


	if(object->getY() + object->getHeight() > viewHeight){
		setViewHeight(object->getY() + object->getHeight());
	}

	drawQueue.push_back(object);

	return true;
}


/*add the given control to the view*/
bool View::addControl(Control *control){
	if(std::find(controls.begin(), controls.end(), control) != controls.end()){
		return false;
	}

	if(horizontalBar != NULL){
		control->setOffsetX(getOffsetX() - horizontalBar->getPos() + xPos);
	}else{
		control->setOffsetX(getOffsetX() + xPos);
	}

	if(verticalBar != NULL){
		control->setOffsetY(getOffsetY() - verticalBar->getPos() + yPos);
	}else{
		control->setOffsetY(getOffsetY() + yPos);
	}

	
	if(control->getX() + control->getWidth() > viewWidth && !control->getRightLock()){
		setViewWidth(control->getX() + control->getWidth());
	}


	if(control->getY() + control->getHeight() > viewHeight && !control->getBottomLock()){
		setViewHeight(control->getY() + control->getHeight());
	}

	control->setWindow(this->getWindow());
	controls.push_back(control);

	return true;
}

/*remove the given control from the view*/
bool View::removeControl(Control *control){
	for(int i=0;i < controls.size();i++){
		if(controls.at(i) == control){
			controls.erase(controls.begin() + i);
			return true;
		}
	}

	return false;
}


/*try to handle the given event*/
bool View::handleEvent(Event *event){
	if(event->getControl() != NULL){
		setActive(false);
		return true;
	}

	bool ret = false;

	Control *activeControl = getActiveControl();

	if(verticalBar != NULL){
		verticalBar->handleEvent(event);

		if(event->getControl() == verticalBar){
			if(verticalBar->getActive()){
				setActive(true);
				setActive(verticalBar);
			}else{
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}

	}

	if(horizontalBar != NULL){
		horizontalBar->handleEvent(event);

		if(event->getControl() == horizontalBar){
			if(horizontalBar->getActive()){
				setActive(true);
				setActive(horizontalBar);
			}else{
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}

	}

	if(activeControl != NULL && activeControl->getVisible()){
		activeControl->handleEvent(event);

		if(event->getControl() == activeControl){
			if(activeControl->getActive()){
				setActive(true);
			}else{
				setActive(false);
			}

			update();
			event->setControl(this);
			ret = true;
		}

	}


	for(int i=controls.size()-1;i>=0;i--){

		if(controls.at(i) != activeControl && controls.at(i)->getVisible()){
			controls.at(i)->handleEvent(event);

			if(event->getControl() == controls.at(i)){
				if(controls.at(i)->getActive()){
					setActive(true);
					setActiveControl(controls.at(i));
				}else{
					setActive(false);
				}

				update();
				event->setControl(this);
				ret = true;
			}
		}

	}


	if(event->getType() == e_SCROLL && (verticalBar != NULL || horizontalBar != NULL)){
		if(boundsCheck(event->getX(), event->getY())){
			if((event->getScrollDirection() == w_UP && event->getModifier() == m_NONE) ||
					(event->getScrollDirection() == w_LEFT && event->getModifier() == m_SHIFT)){

				if(verticalBar != NULL){
					if(verticalBar->scrollUp()){
						update();
					}
				}

			}else if((event->getScrollDirection() == w_DOWN && event->getModifier() == m_NONE) ||
								(event->getScrollDirection() == w_RIGHT && event->getModifier() == m_SHIFT)){

				if(verticalBar != NULL){
					if(verticalBar->scrollDown()){
						update();
					}
				}

			}else if((event->getScrollDirection() == w_LEFT && event->getModifier() == m_NONE) ||
								(event->getScrollDirection() == w_UP && event->getModifier() == m_SHIFT)){

				if(horizontalBar != NULL){
					if(horizontalBar->scrollUp()){
						update();
					}
				}

			}else if((event->getScrollDirection() == w_RIGHT && event->getModifier() == m_NONE) ||
								(event->getScrollDirection() == w_DOWN && event->getModifier() == m_SHIFT)){

				if(horizontalBar != NULL){
					if(horizontalBar->scrollDown()){
						update();
					}
				}

			}

			event->setControl(this);
			ret = true;
		}

	}

	return ret;
}


/*update the theme of the control*/
bool View::updateTheme(){

	if(verticalBar){
		verticalBar->updateTheme();
	}

	if(horizontalBar){
		horizontalBar->updateTheme();
	}

	if(scrollCorner != NULL){
		if(darkMode){
			scrollCorner->setFillColor(*darkTheme[4]);
		}else{
			scrollCorner->setFillColor(*lightTheme[4]);
		}
	}

	for(int i=0;i<controls.size();i++){
		controls.at(i)->updateTheme();
	}


	redraw = true;
	return true;
}



/*free all the memory used*/
void View::free(){
	SDL_DestroyTexture(view);

	if(verticalBar != NULL){
		verticalBar->free();
		delete verticalBar;
	}

	if(horizontalBar != NULL){
		horizontalBar->free();
		delete horizontalBar;
	}

	if(scrollCorner != NULL){
		scrollCorner->free();
		delete scrollCorner;
	}

	setTexture(NULL);
}

/*check is the given X and Y positions are within the bounds of the View*/
bool View::boundsCheck(int x, int y){
	x -= offsetX;
	y -= offsetY;

	return x >= xPos && x < xPos + width && y >= yPos && y < yPos + height;
}




/*
	update anything that needs updated
	the control decides when to update itself
*/
void View::update(){

	if(verticalBar != NULL){
		for(int i=0;i<drawQueue.size();i++){
			drawQueue.at(i)->setOffsetY(getOffsetY() - verticalBar->getPos() + yPos);
		}

		for(int i=0;i<controls.size();i++){
			controls.at(i)->setOffsetY(getOffsetY() - verticalBar->getPos() + yPos);
		}
	}else{
		for(int i=0;i<drawQueue.size();i++){
			drawQueue.at(i)->setOffsetY(getOffsetY() + yPos);
		}

		for(int i=0;i<controls.size();i++){
			controls.at(i)->setOffsetY(getOffsetY() + yPos);
		}
	}

	if(horizontalBar != NULL){
		for(int i=0;i<drawQueue.size();i++){
			drawQueue.at(i)->setOffsetX(getOffsetX() - horizontalBar->getPos() + xPos);
		}

		for(int i=0;i<controls.size();i++){
			controls.at(i)->setOffsetX(getOffsetX() - horizontalBar->getPos() + xPos);
		}
	}else{
		for(int i=0;i<drawQueue.size();i++){
			drawQueue.at(i)->setOffsetX(getOffsetX() + xPos);
		}

		for(int i=0;i<controls.size();i++){
			controls.at(i)->setOffsetX(getOffsetX() + xPos);
		}
	}


	redraw = true;
}

/*update the texture of the View to be printed later*/
bool View::updateTexture(SDL_Renderer *renderer){

	if(view == NULL){
		view = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, viewWidth, viewHeight);
	}

	if(view){
		SDL_Texture *oldTarget = SDL_GetRenderTarget(renderer);

		SDL_SetRenderTarget(renderer, view);

		SDL_SetTextureBlendMode(view, SDL_BLENDMODE_NONE);
		SDL_SetRenderDrawColor(renderer, clearColor.getRed(), clearColor.getGreen(), clearColor.getBlue(), clearColor.getAlpha());
		SDL_RenderClear(renderer);

		for(int i=0;i<controls.size();i++){
			if(controls.at(i)->getVisible()){
				controls.at(i)->draw(renderer);
			}
		}

		for(int i=0;i<drawQueue.size();i++){
			drawQueue.at(i)->draw(renderer);
		}

		SDL_SetRenderTarget(renderer, oldTarget);


		SDL_Rect dest;
		SDL_Rect src;

		if(verticalBar && horizontalBar){
			dest = {0, 0, (int)width - verticalBar->getWidth(), (int)height - horizontalBar->getHeight()};
			src.x = horizontalBar->getPos();
			src.y = verticalBar->getPos();

			if(src.x + width - verticalBar->getWidth() > viewWidth){
				src.x = viewWidth - (width - verticalBar->getWidth());
			}

			if(src.y + height - horizontalBar->getHeight() > viewHeight){
				src.y = viewHeight - (height - horizontalBar->getHeight());
			}

		}else if(verticalBar){
			dest = {0, 0, (int)width - verticalBar->getWidth(), (int)height};
			src.x = 0;
			src.y = verticalBar->getPos();

			if(src.y + height > viewHeight){
				src.x = viewHeight - height;
			}

		}else if(horizontalBar){
			dest = {0, 0, (int)width, (int)height - horizontalBar->getHeight()};
			src.x = horizontalBar->getPos();
			src.y = 0;

			if(src.x + width > viewWidth){
				src.x = viewWidth - width;
			}

		}else{
			dest = {0, 0, (int)width, (int)height};
			src.x = 0;
			src.y = 0;
		}

		src.w = dest.w;
		src.h = dest.h;

		SDL_RenderCopy(renderer, view, &src, &dest);

		if(verticalBar != NULL && horizontalBar != NULL){
			verticalBar->draw(renderer);
			horizontalBar->draw(renderer);

			scrollCorner->draw(renderer);
		}else if(verticalBar != NULL){
			verticalBar->draw(renderer);
		}else if(horizontalBar != NULL){
			horizontalBar->draw(renderer);
		}

		return true;
	}

	return false;
}
