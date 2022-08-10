#include <iostream>
#include <stb_image.h>
#include <SDL.h>
#include <map>

#include "Texture.hpp"
#include "Images.hpp"
#include "2D/Renderer.hpp"

using namespace std;

struct TextureInfo {
    unsigned char* data;
    unsigned int size;
};

static const map<TextureType, TextureInfo> images {
    {TextureType::Button, {Image::ButtonBlue_png, Image::ButtonBlue_png_len}},
    {TextureType::ButtonHover, {Image::ButtonBlueHover_png, Image::ButtonBlueHover_png_len}},
    {TextureType::CheckButton, {Image::CheckBox_png, Image::CheckBox_png_len}},
    {TextureType::CheckButtonSelect, {Image::CheckMark_png, Image::CheckMark_png_len}},

    {TextureType::RadioButtonBlue, {Image::RdbtnBlue_png, Image::RdbtnBlue_png_len}},
    {TextureType::RadioButtonDisabled, {Image::RdbtnDisable_png, Image::RdbtnDisable_png_len}},
    {TextureType::RadioButtonEmpty, {Image::RdbtnEmpty_png, Image::RdbtnEmpty_png_len}},
    {TextureType::RadioButtonGreen, {Image::RdbtnGreen_png, Image::RdbtnGreen_png_len}},
    {TextureType::RadioButtonRed, {Image::RdbtnRed_png, Image::RdbtnRed_png_len}},
    {TextureType::RadioButtonYellow, {Image::RdbtnYellow_png, Image::RdbtnYellow_png_len}}
};

Texture* Texture::loadTexture(TextureType type, TextureManager* manager) {
    TextureInfo info = images.at(type);
    int width, height, channels;
    rgb* stbiData = stbi_load_from_memory(info.data, info.size, &width, &height, &channels, 4);

    if (!stbiData) {
        cerr << "Konnte Textur nicht laden" << endl;
        return NULL;
    }

    Texture* result = new Texture(width, height, channels, manager, type);

    int x = 0, y = 0, maxIndex = width * height * channels;
    Color c;
    if (channels == 4) {
        for (int index = 0; index < maxIndex; index+= 4) {
            c.r = stbiData[index];
            c.g = stbiData[index + 1];
            c.b = stbiData[index + 2];
            c.a = stbiData[index + 3];

            //Daten schreiben
            result->at(x, y) = c;
            x++;
            if (x == width) {
                x = 0;
                y++;
            }
        }
    } else { //3 Channel, alpha Wert immer 255
        c.a = 255;
        for (int index = 0; index < maxIndex; index+= 3) {
            c.r = stbiData[index];
            c.g = stbiData[index + 1];
            c.b = stbiData[index + 2];

            //Daten schreiben
            result->at(x, y) = c;
            x++;
            if (x == width) {
                x = 0;
                y++;
            }
        }
    }

    stbi_image_free(stbiData);

    return result;
}

SDL_Surface* Texture::loadIcon() {
    int width, height, channels;
    rgb* stbiData = stbi_load_from_memory(Image::Icon_png, Image::Icon_png_len, &width, &height, &channels, 4);

    if (!stbiData) {
        cerr << "Konnte Icon nicht laden" << endl;
        return NULL;
    }

    //SDL_Surface* data = SDL_CreateRGBSurface(0, width, height, channels * 8, 0xFF'00'00'00, 0x00'FF'00'00, 0x00'00'FF'00, 0x00'00'00'FF);
    SDL_Surface* data = SDL_CreateRGBSurfaceWithFormat(0, width, height, channels * 8, SDL_PIXELFORMAT_RGBA32);
    if (!data) {
        cerr << "Konnte Icon nicht erstellen: " << SDL_GetError() << endl;
        return NULL;
    }

    data->pixels = stbiData;

    return data;
}