#include <iostream>
#include <glm/vec2.hpp>

#include "Geometry.hpp"
#include "2D/Renderer.hpp"
#include "App/App.hpp"
#include "Window/Window.hpp"

using namespace std;
using namespace glm;

const Rect Rect::emptyRect;
const vec2 difference(0.5, 0.5);

bool Rect::isInside(const int& x, const int& y) {
    return x >= this->x && x < this->x + width && y >= this->y && y < this->y + height;
}

bool Rect::isInside(const Point& p) {
    return isInside(p.x, p.y);
}

vec2 Point::toCoord(Main* main) {
    vec2 normal((float) x / (float) renderWidth, (float) y / (float) renderHeight);
    (normal -= difference) *= 2; //Ergibt offset von -1 bis 1

    vec2 scalar(-main->width, main->width / main->window->ratio); //Offset skaliert mit aktuellem Zoom

    vec2 offset = normal * scalar;
    offset *= main->deltaTime;

    return main->focusCenter + offset;
}

bool Point::operator == (const Point& p) const {
    return x == p.x && y == p.y;
}

bool Point::operator != (const Point& p) const {
    return x != p.x || y != p.y;
}

Point Point::operator - (const Point& p) const {
    return {p.x - x, p.y - y};
}