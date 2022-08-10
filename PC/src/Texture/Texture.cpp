#include <iostream>

#include "Texture.hpp"
#include "2D/Renderer.hpp"

using namespace std;

Texture::Texture(const int& width, const int& height, const int& channel, TextureManager* manager, TextureType type)
:width(width), height(height), channel(channel), manager(manager), type(type) {
        data.resize(width * height);
    }

Texture::~Texture() {
    data.clear();
    if (manager) manager->removeTexture(this);
}

const std::vector<Color>& Texture::getData() const {
    return data;
}

Color& Texture::at(const int& x, const int& y) {
    return data.at(y * width + x);
}