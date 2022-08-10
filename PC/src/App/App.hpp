#pragma once

#include <string>
#include <SDL_events.h>
#include <glm/vec2.hpp>

#include "2D/Geometry.hpp"

class Window;
class Shader;
class Buffer;
class FontManager;
class Renderer;
class TextureManager;
class Overlay;

enum SaveState : unsigned char {None, Started, Render};

class Main {
    friend class Point;
    private:
        bool running = false;
        SDL_Event event;
        glm::vec2 center, focusCenter;
        float width, height;
        Point focusPoint, lastPoint;
        unsigned char pressedKeys = 0, saveState = None;
        std::string filename;

        bool onInit();
        void calculateDeltaTime(const bool& startFrame);
        void onEvent();
        void onLoop();
        void onRender();
        void onCleanup();
        bool isKeyPressed(const unsigned char& bit);
        void setKeyPressed(const unsigned char& bit, const bool& value);
        void saveImage();
    
    public:
        double deltaTime;
        int fps;
        Window* window;
        Shader* mainShader, *overlayShader;
        Buffer* buffer;
        FontManager* fontMan;
        TextureManager* textureMan;
        Renderer* renderer;
        Overlay* ui;

        void startSave();
        void resetCoords();
        int onExecute();
        void quit();
};