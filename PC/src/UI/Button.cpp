#include <iostream>

#include "Widget.hpp"
#include "Texture/Texture.hpp"
#include "App/App.hpp"
#include "2D/Font.hpp"
#include "2D/Renderer.hpp"
#include "UIEvents.hpp"

using namespace std;

void AbstractButton::triggerEvent() {
    if (click) click->click(this);
}

AbstractButton::~AbstractButton() {
    delete click;
}

Button::Button(const Rect& bounds, Main* main, const String32& text, ClickEvent* click)
: AbstractButton(click), Widget(WidgetType::Button, bounds, main), text(text) {
    image = main->textureMan->getTexture(TextureType::Button);
    hoverImage = main->textureMan->getTexture(TextureType::ButtonHover);
    font = main->fontMan->getFont(FontType::Arial);
}

void Button::onLoop() {
    isHovering = false;
}

void Button::onRender() {
    main->renderer->setColor(Colors::Black);
    if (isHovering) {
        main->renderer->drawTexture(hoverImage, Rect::emptyRect, bounds.x, bounds.y + 4);
        main->renderer->drawText(font, text, bounds.x + 10, bounds.y + 0.7 * bounds.height + 4, 35);
    } else {
        main->renderer->drawTexture(image, Rect::emptyRect, bounds.x, bounds.y);
        main->renderer->drawText(font, text, bounds.x + 10, bounds.y + 0.7 * bounds.height, 35);
    }
}

void Button::onClick(const Point& pos) {
    triggerEvent();
}

void Button::onHover(const Point& pos) {
    isHovering = true;
}
