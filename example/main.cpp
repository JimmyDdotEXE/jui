#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>

#include "General.h"
#include "Window.h"
#include "Controls.h"


/*tell JUI where to find the font and icon files*/
std::string fontLocation = "SourceCodePro-Regular.ttf";
std::string iconLocation = "Untitled.png";


/*derived button class to change the title of the window*/
class TitleButton : public Button{
	public:
		TitleButton(int x, int y, Window *win, TextBox *text) : Button(x, y, "Set Title"){
			window = win;
			textBox = text;

			hasLeftClick = true;
		}

		bool leftClick(){
			window->setTitle(textBox->getText());

			return window->getTitle() == textBox->getText();
		}

	protected:
		Window *window;
		TextBox *textBox;
};


/*entry point of the function*/
int main(int argc, char *argv[]){

	/*load the color scheme of the program*/
	loadPalette();

	/*start the JUI backend*/
	start();

	/*creat the window*/
	Window win(480, 360, "Test Window");


	/*define all the needed controls*/
	View *v = new View(0, 0, 480, 360);
	v->setLeftLock(true);
	v->setRightLock(true);
	v->setTopLock(true);
	v->setBottomLock(true);
	v->setLockResize(true);


	double sliderDouble = 0;
	Slider *slider = new Slider(10, 10, 360 - 36, 0, 100, &sliderDouble, HORIZONTAL);
	slider->setTopLock(true);
	slider->setBottomLock(true);


	Label *label = new Label(slider->getX() + slider->getWidth() + 10, 10, 20, numString(0));


	TextBox *tex = new TextBox(label->getX(), label->getY() + label->getHeight() + 10, 120, 24, "Window Title");
	tex->setText(win.getTitle());


	TitleButton *button = new TitleButton(tex->getX(), tex->getY() + tex->getHeight() + 10, &win, tex);
	button->setX(tex->getX() + (tex->getWidth() - button->getWidth())/2);



	CheckBoxGroup *visibleControls = new CheckBoxGroup(tex->getX() + tex->getWidth() + 10, 10, "Visible Controls",
																											{"Label", "Slider", "Button", "TextBox",
																												"RadioButtonGroup", "ComboBox", "Toggle"});
	visibleControls->setSelection({"Label", "Slider", "Button", "TextBox",
																	"RadioButtonGroup", "ComboBox", "Toggle"});


	RadioButtonGroup *rad = new RadioButtonGroup(slider->getX() + slider->getWidth() + 10, 0, "Accent Color",
																								{"Yellow", "Orange", "Red", "Magenta",
																									"Violet", "Blue", "Cyan", "Green"});
	rad->setY(win.getHeight() - 10 - rad->getHeight());
	rad->setBottomLock(true);
	rad->setSelection("Cyan");


	ComboBox *combo = new ComboBox(rad->getX(), 0);
	combo->setY(rad->getY() - 10 - combo->getHeight());
	combo->setBottomLock(true);
	combo->setList({"Yellow", "Orange", "Red", "Magenta", "Violet", "Blue", "Cyan", "Green"});
	combo->setSelection({"Cyan"});


	Toggle *darkToggle = new Toggle(0, 10, &darkMode);
	darkToggle->setX(v->getWidth() - darkToggle->getWidth() - 10);
	darkToggle->setRightLock(true);


	/*add the view to the window*/
	win.addControl(v);

	/*add all the controls to the view*/
	v->addControl(tex);
	v->addControl(slider);
	v->addControl(label);
	v->addControl(rad);
	v->addControl(combo);
	v->addControl(darkToggle);
	v->addControl(button);
	v->addControl(visibleControls);


	/*run the program as long as the window is open*/
	while(win.isReady()){

		/*handle the window events*/
		win.handleEvents();


		/*clear the window and view based on the darkmode setting*/
		if(darkMode){
			win.clear(*darkTheme[0]);
			v->clear(*darkTheme[0]);
		}else{
			win.clear(*lightTheme[0]);
			v->clear(*lightTheme[0]);
		}


		/*set highlight color based on changes to the RadioButtons and ComboBox*/
		if((rad->getSelection() == "Yellow" || combo->getSelection().at(0) == "Yellow") && accent != &highlightColor[0]){
			accent = &highlightColor[0];
			win.updateTheme();

			rad->setSelection("Yellow");
			combo->setSelection({"Yellow"});
		}else if((rad->getSelection() == "Orange" || combo->getSelection().at(0) == "Orange") && accent != &highlightColor[1]){
			accent = &highlightColor[1];
			win.updateTheme();

			rad->setSelection("Orange");
			combo->setSelection({"Orange"});
		}else if((rad->getSelection() == "Red" || combo->getSelection().at(0) == "Red") && accent != &highlightColor[2]){
			accent = &highlightColor[2];
			win.updateTheme();

			rad->setSelection("Red");
			combo->setSelection({"Red"});
		}else if((rad->getSelection() == "Magenta" || combo->getSelection().at(0) == "Magenta") && accent != &highlightColor[3]){
			accent = &highlightColor[3];
			win.updateTheme();

			rad->setSelection("Magenta");
			combo->setSelection({"Magenta"});
		}else if((rad->getSelection() == "Violet" || combo->getSelection().at(0) == "Violet") && accent != &highlightColor[4]){
			accent = &highlightColor[4];
			win.updateTheme();

			rad->setSelection("Violet");
			combo->setSelection({"Violet"});
		}else if((rad->getSelection() == "Blue" || combo->getSelection().at(0) == "Blue") && accent != &highlightColor[5]){
			accent = &highlightColor[5];
			win.updateTheme();

			rad->setSelection("Blue");
			combo->setSelection({"Blue"});
		}else if((rad->getSelection() == "Cyan" || combo->getSelection().at(0) == "Cyan") && accent != &highlightColor[6]){
			accent = &highlightColor[6];
			win.updateTheme();

			rad->setSelection("Cyan");
			combo->setSelection({"Cyan"});
		}else if((rad->getSelection() == "Green" || combo->getSelection().at(0) == "Green") && accent != &highlightColor[7]){
			accent = &highlightColor[7];
			win.updateTheme();

			rad->setSelection("Green");
			combo->setSelection({"Green"});
		}


		/*hide all controls*/
		tex->setVisible(false);
		slider->setVisible(false);
		label->setVisible(false);
		rad->setVisible(false);
		combo->setVisible(false);
		darkToggle->setVisible(false);
		button->setVisible(false);

		/*show controls based on which CheckBoxes are selected*/
		std::vector<std::string> selected = visibleControls->getSelection();
		for(int i=0;i<selected.size();i++){
			if(selected.at(i) == "Label"){
				label->setVisible(true);
			}else if(selected.at(i) == "Slider"){
				slider->setVisible(true);
			}else if(selected.at(i) == "Button"){
				button->setVisible(true);
			}else if(selected.at(i) == "TextBox"){
				tex->setVisible(true);
			}else if(selected.at(i) == "RadioButtonGroup"){
				rad->setVisible(true);
			}else if(selected.at(i) == "ComboBox"){
				combo->setVisible(true);
			}else if(selected.at(i) == "Toggle"){
				darkToggle->setVisible(true);
			}
		}


		/*set the label text based on the value of the slider*/
		std::ostringstream stream;
		stream << std::fixed;
		stream << std::setprecision(2);
		stream << sliderDouble;

		label->setText(stream.str());


		/*draw all controls*/
		win.drawControls();

		/*post the current frame to the window*/
		win.postFrame();
	}


	/*stop the JUI backend*/
	stop();

	return 0;
}
