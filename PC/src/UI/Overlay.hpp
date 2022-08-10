#pragma once
#include <vector>

#include "2D/Geometry.hpp"

class Widget;
class AbstractButton;
class Button;
class Main;
class CheckButton;
class RadioButton;
class RadioButtonGroup;
class ColorSelectionEvent;
class ResetClickEvent;
class Font;

class Overlay {
    friend class ColorSelectionEvent;
    friend class ResetClickEvent;
    private:
        std::vector<Widget*> widgets;
        Main* main;
        Rect bounds;

        Button* saveButton, *quitButton, *resetButton;
        CheckButton* invert;
        RadioButtonGroup* firstGroup, *secondGroup;
        RadioButton* firstNone, *firstRed, *firstGreen, *firstBlue;
        RadioButton* secondNone, *secondRed, *secondGreen, *secondBlue;

        int firstColor = 0, secondColor = 0, invertBit = 0;
        Font* font;
    
    public:
        int colorMap = 0;

        Overlay(Main* main);
        ~Overlay();

        void onLoop();
        void onRender();
        bool isInside();
};