#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "JUI.h"

#include "Utility.h"
#include "Window.h"
#include "Controls.h"


/*tell JUI where to find the font and icon files*/
std::string fontLocation = "SourceCodePro-Regular.ttf";
std::string iconLocation = "Untitled.png";


uint mouseDeadZone = 10;


/*entry point of the function*/
int main(int argc, char *argv[]){
	/*load the color scheme of the program*/
	loadPalette();

	/*start the JUI backend*/
	start();

	/*create the windows*/
	Window win1(480, 360, "Test Window");
	mountWindow(&win1);

	Window win2(480, 360, "Second Window");
	mountWindow(&win2);

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
	tex->setText(win1.getTitle());


	Button *button = new Button(tex->getX(), tex->getY() + tex->getHeight() + 10, "Set Title",
			[&win=win1, tex](){
				win.setTitle(tex->getText());
				return win.getTitle() == tex->getText();
			});
	button->setX(tex->getX() + (tex->getWidth() - button->getWidth())/2);



	CheckBoxGroup *visibleControls = new CheckBoxGroup(tex->getX() + tex->getWidth() + 10, 10, "Visible Controls",
																											{"Label", "Slider", "Button", "TextBox",
																												"RadioButtonGroup", "ComboBox", "Toggle"});
	visibleControls->setSelection({"Label", "Slider", "Button", "TextBox",
																	"RadioButtonGroup", "ComboBox", "Toggle"});


	RadioButtonGroup *rad = new RadioButtonGroup(slider->getX() + slider->getWidth() + 10, 0, "Accent Color",
																								{"Yellow", "Orange", "Red", "Magenta",
																									"Violet", "Blue", "Cyan", "Green"});
	rad->setY(win1.getHeight() - 10 - rad->getHeight());
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

	Toggle *invertToggle = new Toggle(darkToggle->getX(), darkToggle->getY() + darkToggle->getHeight() + 10, &invertedMode);
	invertToggle->setRightLock(true);


	/*add the view to the main window*/
	win1.addControl(v);

	/*add all the controls to the view*/
	v->addControl(tex);
	v->addControl(rad);
	v->addControl(combo);
	v->addControl(darkToggle);
	v->addControl(invertToggle);
	v->addControl(button);
	v->addControl(visibleControls);

	//add slider and label to the second window
	win2.addControl(slider);
	win2.addControl(label);

	/*run the program as long as the window is open*/
	while(ready()){

		/*check window themes and handle events*/
		framePrep();


		/*clear the view based on the darkmode setting*/
		if(darkMode){
			v->clear(*darkTheme[0]);
		}else{
			v->clear(*lightTheme[0]);
		}


		/*set highlight color based on changes to the RadioButtons and ComboBox*/
		if((rad->getSelection() == "Yellow" || combo->getSelection().at(0) == "Yellow") && accent != &highlightColor[0]){
			accent = &highlightColor[0];
			rad->setSelection("Yellow");
			combo->setSelection({"Yellow"});
		}else if((rad->getSelection() == "Orange" || combo->getSelection().at(0) == "Orange") && accent != &highlightColor[1]){
			accent = &highlightColor[1];
			rad->setSelection("Orange");
			combo->setSelection({"Orange"});
		}else if((rad->getSelection() == "Red" || combo->getSelection().at(0) == "Red") && accent != &highlightColor[2]){
			accent = &highlightColor[2];
			rad->setSelection("Red");
			combo->setSelection({"Red"});
		}else if((rad->getSelection() == "Magenta" || combo->getSelection().at(0) == "Magenta") && accent != &highlightColor[3]){
			accent = &highlightColor[3];
			rad->setSelection("Magenta");
			combo->setSelection({"Magenta"});
		}else if((rad->getSelection() == "Violet" || combo->getSelection().at(0) == "Violet") && accent != &highlightColor[4]){
			accent = &highlightColor[4];
			rad->setSelection("Violet");
			combo->setSelection({"Violet"});
		}else if((rad->getSelection() == "Blue" || combo->getSelection().at(0) == "Blue") && accent != &highlightColor[5]){
			accent = &highlightColor[5];
			rad->setSelection("Blue");
			combo->setSelection({"Blue"});
		}else if((rad->getSelection() == "Cyan" || combo->getSelection().at(0) == "Cyan") && accent != &highlightColor[6]){
			accent = &highlightColor[6];
			rad->setSelection("Cyan");
			combo->setSelection({"Cyan"});
		}else if((rad->getSelection() == "Green" || combo->getSelection().at(0) == "Green") && accent != &highlightColor[7]){
			accent = &highlightColor[7];
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

		/*draw controls and post window frames*/
		frameCleanUp();
	}


	/*stop the JUI backend*/
	stop();

	return 0;
}
