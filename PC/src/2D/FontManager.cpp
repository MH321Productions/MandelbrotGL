#include <iostream>

#include "Font.hpp"

using namespace std;

FontManager::FontManager() {
    //FreeType initialisieren
    isInitialized = checkFTError(FT_Init_FreeType(&library), "Konnte FreeType nicht initialisieren");
}

FontManager::~FontManager() {
    blockRemoval = true;

    for (Font* f: loadedFonts) delete f;

    loadedFonts.clear();

    //FreeType stoppen
    checkFTError(FT_Done_FreeType(library), "Konnte FreeType nicht beenden");
}

void FontManager::removeFont(Font* font) {
    if (!blockRemoval) {
        for (vector<Font*>::iterator it = loadedFonts.begin(); it != loadedFonts.end(); it++) {
            if (*it == font) {
                loadedFonts.erase(it);
                break;
            }
        }
    }
}

Font* FontManager::getFont(FontType type) {
    for (Font* f: loadedFonts) {
        if (f != NULL && f->type == type) return f;
    }

    if (type == FontType::None) return NULL;

    Font* load = new Font(type, library, this);
    loadedFonts.push_back(load);

    return load;
}

bool FontManager::checkFTError(const int& error, const std::string& errorMessage) {
    if (error) {
        cerr << errorMessage << ": " << FT_Error_String(error) << endl;
        return false;
    }

    return true;
}