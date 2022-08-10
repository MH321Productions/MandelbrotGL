#include "Renderer.hpp"

const Color Colors::Transparent;
const Color Colors::Black(0, 0, 0, 0xFF);
const Color Colors::White(0xFF, 0xFF, 0xFF, 0xFF);
const Color Colors::Red(0xFF, 0, 0, 0xFF);
const Color Colors::Green(0, 0xFF, 0, 0xFF);
const Color Colors::Blue(0, 0, 0xFF, 0xFF);
const Color Colors::Yellow(0xFF, 0xFF, 0, 0xFF);

Color::Color(const int& r, const int& g, const int& b, const int& a) {
    checkValue(r, Color::r);
    checkValue(g, Color::g);
    checkValue(b, Color::b);
    checkValue(a, Color::a);
}

Color::Color(const double& r, const double& g, const double& b, const double& a) {
    checkValue(r, Color::r);
    checkValue(g, Color::g);
    checkValue(b, Color::b);
    checkValue(a, Color::a);
}

void Color::checkValue(const int& input, rgb& rgba) {
    if (input < 0) rgba = 0;
    else if (input > 0xFF) rgba = 0xFF;
    else rgba = (rgb) input;
}

Color& Color::operator = (const Color& c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;

    return *this;
}

Color& Color::operator = (Color& c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;

    return *this;
}

Color Color::operator + (Color& c) const {
    return {r + c.r, g + c.g, b + c.b, a + c.a};
}

Color Color::operator + (Color c) const {
    return {r + c.r, g + c.g, b + c.b, a + c.a};
}

Color& Color::operator += (Color& c) {
    r += c.r;
    g += c.g;
    b += c.b;
    a += c.a;

    return *this;
}

Color Color::operator - (Color& c) const {
    return {r - c.r, g - c.g, b - c.b, a - c.a};
}

Color& Color::operator -= (Color& c) {
    r -= c.r;
    g -= c.g;
    b -= c.b;
    a -= c.a;

    return *this;
}

Color Color::operator * (const rgb& scalar) const {
    return {r * scalar, g * scalar, b * scalar, a * scalar};
}

Color Color::operator * (const double& scalar) const {
    return {r * scalar, g * scalar, b * scalar, a * scalar};
}

Color& Color::operator *= (const rgb& scalar) {
    checkValue(r * scalar, r);
    checkValue(g * scalar, g);
    checkValue(b * scalar, b);
    checkValue(a * scalar, a);

    return *this;
}

Color Color::operator / (const rgb& scalar) const {
    return {r / scalar, g / scalar, b / scalar, a / scalar};
}

Color& Color::operator /= (const rgb& scalar) {
    checkValue(r / scalar, r);
    checkValue(g / scalar, g);
    checkValue(b / scalar, b);
    checkValue(a / scalar, a);

    return *this;
}

bool Color::operator == (const Color& c) const {
    return r == c.r && g == c.g && b == c.b && a == c.a;
}