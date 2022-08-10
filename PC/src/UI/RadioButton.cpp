#include <iostream>

#include "Widget.hpp"
#include "Texture/Texture.hpp"
#include "App/App.hpp"
#include "2D/Font.hpp"
#include "2D/Renderer.hpp"
#include "UIEvents.hpp"

using namespace std;

RadioButton* RadioButtonGroup::getSelectedButton() const {
    for (RadioButton* b: buttons) if (b && b->selected) return b;

    return NULL;
}

void RadioButtonGroup::setSelectedButton(RadioButton* btn) {
    for (RadioButton* b: buttons) {
        if (b == btn) b->selected = true;
        else b->selected = false;
    }
}

void RadioButtonGroup::addButton(RadioButton* btn) {
    buttons.push_back(btn);
}

RadioButton::RadioButton(const Rect& bounds, Main* main, const String32& text, const bool& isSelected, ClickEvent* event,
TextureType selectedType, RadioButtonGroup* group) : Widget(WidgetType::RadioButton, bounds, main), AbstractButton(event),
SelectionButton(isSelected), group(group), text(text) {

    image = main->textureMan->getTexture(TextureType::RadioButtonEmpty);
    disabledImage = main->textureMan->getTexture(TextureType::RadioButtonDisabled);
    selectedImage = main->textureMan->getTexture(selectedType);
    font = main->fontMan->getFont(FontType::Arial);

    group->addButton(this);
    event->setUserdata(this);
}

void RadioButton::onRender() {
    main->renderer->setColor(Colors::Black);

    Texture* temp;
    if (enabled) {
        if (selected) temp = selectedImage;
        else temp = image;
    } else temp = disabledImage;

    main->renderer->drawTexture(temp, Rect::emptyRect, bounds.x, bounds.y);

    main->renderer->drawText(font, text, bounds.x + image->width + 5, bounds.y + 0.75 * bounds.height, 35);
}

void RadioButton::onClick(const Point& pos) {
    if (enabled){
        if (group) group->setSelectedButton(this);
        triggerEvent();
    }
}

void RadioButton::setSelected(const bool& newSelection, const bool& triggerEvent) {
    if (enabled) {
        if (newSelection && group) group->setSelectedButton(this);
        else if (!newSelection && group && group->getSelectedButton() == this) group->setSelectedButton(NULL);

        selected = newSelection;
        if (triggerEvent) AbstractButton::triggerEvent();
    }
}