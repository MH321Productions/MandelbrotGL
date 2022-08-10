#include <iostream>

#include "Widget.hpp"
#include "UIEvents.hpp"
#include "Texture/Texture.hpp"
#include "App/App.hpp"
#include "2D/Font.hpp"
#include "2D/Renderer.hpp"

using namespace std;

CheckButton::CheckButton(const Rect& bounds, Main* main, const String32& text, const bool& isSelected, ClickEvent* event)
: SelectionButton(isSelected), AbstractButton(event), Widget(WidgetType::CheckButton, bounds, main), text(text) {
    image = main->textureMan->getTexture(TextureType::CheckButton);
    selectedImage = main->textureMan->getTexture(TextureType::CheckButtonSelect);
    font = main->fontMan->getFont(FontType::Arial);
}

void CheckButton::onRender() {
    main->renderer->setColor(Colors::Black);
    if (selected) main->renderer->drawTexture(selectedImage, Rect::emptyRect, bounds.x, bounds.y);
    else main->renderer->drawTexture(image, Rect::emptyRect, bounds.x, bounds.y);

    main->renderer->drawText(font, text, bounds.x + image->width + 5, bounds.y + 0.75 * bounds.height, 35);
}

void CheckButton::onClick(const Point& pos) {
    flipSelection();
    triggerEvent();
}