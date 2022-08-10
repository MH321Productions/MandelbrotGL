#include <iostream>
#include <sstream>
#include <map>

#include "Font.hpp"
#include "Fonts.hpp"

using namespace std;

struct FontInfo {
    unsigned char* data;
    unsigned int size;
};

static const map<FontType, FontInfo> fonts {
    {FontType::Future, {Fonts::Future_ttf, Fonts::Future_ttf_len}},
    {FontType::Arial, {Fonts::arial_unicode_ms_ttf, Fonts::arial_unicode_ms_ttf_len}},
    //{FontType::ComicSans, {Fonts::COMIC_ttf, Fonts::COMIC_ttf_len}}
};

Font::Font(FontType type, FT_Library lib, FontManager* manager)
    : type(type), library(lib), width(0), height(0), isLoaded(false), hasUnicode(false), manager(manager) {
    
    FontInfo info = fonts.at(type);

    //Font laden
    isLoaded = FontManager::checkFTError(
        FT_New_Memory_Face(library, info.data, info.size, 0, &face),
        formatType("Konnte Font ", " nicht öffnen")
    );

    //Wenn der Font geladen werden konnte, versuche auf Unicode zu setzen
    if (isLoaded) {
        hasUnicode = FontManager::checkFTError(
            FT_Select_Charmap(face, FT_ENCODING_UNICODE),
            formatType("Der Font ", " kann nicht mit Unicode benutzt werden")
        );
    }
}

Font::~Font() {
    if (isLoaded) {
        FontManager::checkFTError(FT_Done_Face(face), formatType("Konnte Font ", " nicht entladen"));
    }

    if (manager != NULL) manager->removeFont(this);
}

bool Font::setSize(const int& width, const int& height) {
    this->width = width;
    this->height = height;

    return FontManager::checkFTError(
        FT_Set_Pixel_Sizes(face, width, height),
        formatType("Konnte Font ", " nicht skalieren")
    );
}

string Font::formatType(const std::string& before, const std::string& after) {
    ostringstream str;
    str << before << (int) type << after;
    return str.str();
}