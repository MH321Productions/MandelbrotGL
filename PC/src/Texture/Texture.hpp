#pragma once

#include <vector>

using rgb = unsigned char;

struct SDL_Surface;
class Color;

enum class TextureType {
    None,
    RadioButtonDisabled, RadioButtonEmpty, RadioButtonRed, RadioButtonGreen, RadioButtonBlue, RadioButtonYellow,
    Button, ButtonHover,
    CheckButton, CheckButtonSelect
};

class TextureManager;

class Texture {
    friend class TextureManager;
    private:
        std::vector<Color> data;
        TextureManager* manager;
        TextureType type;
        
        Texture(const int& width, const int& height, const int& channel, TextureManager* manager, TextureType type);

        static Texture* loadTexture(TextureType type, TextureManager* manager);
    
    public:
        int width, height, channel;
        Texture() : data({}), width(0), height(0), channel(0) {}
        ~Texture();

        const std::vector<Color>& getData() const;

        Color& at(const int& x, const int& y);
        inline bool isOutside(const int& x, const int& y) const {return x < 0 || y < 0 || x >= width || y >= height;}

        static SDL_Surface* loadIcon();
};

class TextureManager {
    friend class Texture;
    private:
        std::vector<Texture*> loadedTextures;
        bool blockRemoval = false;

        void removeTexture(Texture* texture);
    
    public:
        TextureManager() {}
        ~TextureManager();

        Texture* getTexture(TextureType type);
};