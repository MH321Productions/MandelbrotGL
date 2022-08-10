#include <iostream>

#include "Texture.hpp"
#include "2D/Renderer.hpp"

using namespace std;

Texture* TextureManager::getTexture(TextureType type) {
    for (Texture* t: loadedTextures) {
        if (t->type == type) return t; //Temporär
    }

    Texture* load = Texture::loadTexture(type, this);
    loadedTextures.push_back(load);

    return load;
}

TextureManager::~TextureManager() {
    blockRemoval = true; //Damit sich die Texturen nicht direkt aus der Liste entfernen können

    for (Texture* t: loadedTextures) delete t;

    loadedTextures.clear();

    cout << "TextureManager gelöscht" << endl;
}

void TextureManager::removeTexture(Texture* texture) {
    if (!blockRemoval && texture != NULL) {
        for (vector<Texture*>::iterator it = loadedTextures.begin(); it != loadedTextures.end(); it++) {
            if ((*it) == texture) {
                loadedTextures.erase(it);
                break;
            }
        }
    }
}