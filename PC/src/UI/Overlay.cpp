#include <iostream>

#include "Overlay.hpp"
#include "Widget.hpp"
#include "2D/Font.hpp"
#include "2D/Geometry.hpp"
#include "2D/Renderer.hpp"
#include "App/App.hpp"
#include "Window/Window.hpp"
#include "Texture/Texture.hpp"
#include "UIEvents.hpp"

using namespace std;

class ColorSelectionEvent : public virtual ClickEvent {
    private:
        Overlay* ui;

    public:
        ColorSelectionEvent(Overlay* ui) : ui(ui) {}
        ~ColorSelectionEvent() {}

        virtual void click(AbstractButton* btn) {
            if (btn == ui->firstNone) {
                ui->secondNone->setSelected(true, true);
                ui->secondNone->enabled = false;
                ui->secondRed->enabled = false;
                ui->secondGreen->enabled = false;
                ui->secondBlue->enabled = false;
                ui->firstColor = 0;

            } else if (btn == ui->firstRed || btn == ui->firstGreen || btn == ui->firstBlue) {
                ui->secondNone->enabled = true;
                ui->secondRed->enabled = true;
                ui->secondGreen->enabled = true;
                ui->secondBlue->enabled = true;
                if (!ui->secondNone->enabled) ui->secondNone->setSelected(true, true);
            }

            if (btn == ui->firstRed) {
                ui->secondRed->enabled = false;
                if (ui->secondRed->isSelected()) ui->secondNone->setSelected(true, true);
                ui->firstColor = 1;
            } else if (btn == ui->firstGreen) {
                ui->secondGreen->enabled = false;
                if (ui->secondGreen->isSelected()) ui->secondNone->setSelected(true, true);
                ui->firstColor = 2;
            } else if (btn == ui->firstBlue) {
                ui->secondBlue->enabled = false;
                if (ui->secondBlue->isSelected()) ui->secondNone->setSelected(true, true);
                ui->firstColor = 3;
            }

            if (btn == ui->secondNone) ui->secondColor = 0;
            else if (btn == ui->secondRed) ui->secondColor = 1;
            else if (btn == ui->secondGreen) ui->secondColor = 2;
            else if (btn == ui->secondBlue) ui->secondColor = 3;
            
            if (btn == ui->invert) ui->invertBit = ui->invert->isSelected();
        }
};

Overlay::Overlay(Main* main) : main(main) {
    font = main->fontMan->getFont(FontType::Future);
    bounds = {350, 900, 1230, 180};

    //ButtonGroups erstellen
    firstGroup = new RadioButtonGroup;
    secondGroup = new RadioButtonGroup;

    //Widgets erstellen
    saveButton = new Button({1360, 905, 190, 49}, main, U"Speichern", new ClickEvent([](AbstractButton* btn) {btn->main->startSave();}));
    resetButton = new Button({1360, 965, 190, 49}, main, U"Reset", StandardClickEvents::resetCoords(this));
    quitButton = new Button({1360, 1025, 190, 49}, main, U"Beenden", StandardClickEvents::quit());
    invert = new CheckButton({1060, 972, 230, 36}, main, U"Invertieren", false, new ColorSelectionEvent(this));

    firstNone = new RadioButton({370, 905, 300, 36}, main, U"Keine 1. Farbe", true, new ColorSelectionEvent(this),TextureType::RadioButtonYellow, firstGroup);
    firstRed = new RadioButton({370, 950, 105, 36}, main, U"Rot", false, new ColorSelectionEvent(this), TextureType::RadioButtonRed, firstGroup);
    firstGreen = new RadioButton({370, 995, 125, 36}, main, U"Gr\u00FCn", false, new ColorSelectionEvent(this), TextureType::RadioButtonGreen, firstGroup);
    firstBlue = new RadioButton({370, 1040, 115, 36}, main, U"Blau", false, new ColorSelectionEvent(this), TextureType::RadioButtonBlue, firstGroup);

    secondNone = new RadioButton({710, 905, 300, 36}, main, U"Keine 2. Farbe", true, new ColorSelectionEvent(this), TextureType::RadioButtonYellow, secondGroup);
    secondRed = new RadioButton({710, 950, 105, 36}, main, U"Rot", false, new ColorSelectionEvent(this), TextureType::RadioButtonRed, secondGroup);
    secondGreen = new RadioButton({710, 995, 125, 36}, main, U"Gr\u00FCn", false, new ColorSelectionEvent(this), TextureType::RadioButtonGreen, secondGroup);
    secondBlue = new RadioButton({710, 1040, 115, 36}, main, U"Blau", false, new ColorSelectionEvent(this), TextureType::RadioButtonBlue, secondGroup);
    secondNone->enabled = false;
    secondRed->enabled = false;
    secondGreen->enabled = false;
    secondBlue->enabled = false;

    //Widgets in Liste speichern
    widgets.push_back(saveButton);
    widgets.push_back(resetButton);
    widgets.push_back(quitButton);
    widgets.push_back(invert);
    widgets.push_back(firstNone);
    widgets.push_back(firstRed);
    widgets.push_back(firstGreen);
    widgets.push_back(firstBlue);
    widgets.push_back(secondNone);
    widgets.push_back(secondRed);
    widgets.push_back(secondGreen);
    widgets.push_back(secondBlue);
}

Overlay::~Overlay() {
    for (Widget* w: widgets) delete w;

    delete firstGroup;
    delete secondGroup;

    widgets.clear();
}

void Overlay::onLoop() {
    for (Widget* w: widgets) w->onLoop();

    //Klicken und Hovern
    Point mouse = main->renderer->getRelMousePos();
    
    for (Widget* w: widgets) {
        if (w->bounds.isInside(mouse)) {
            w->onHover(mouse);
            if (main->window->isClicked()) w->onClick(mouse);
        }
    }

    //Farbwert berechnen
    colorMap = 100 * invertBit + 10 * firstColor + secondColor;
}

void Overlay::onRender() {
    main->renderer->setColor(Colors::Yellow);
    main->renderer->fillRect(bounds);

    for (Widget* w: widgets) w->onRender();
    main->renderer->drawFPS(font);
}

bool Overlay::isInside() {
    return bounds.isInside(main->renderer->getRelMousePos());
}