#include <iostream>
#include <SDL.h>
#include <portable-file-dialogs.h>

#include "App.hpp"
#include "Window/Window.hpp"
#include "UI/Overlay.hpp"
//#include "2D/Renderer.hpp"
//#include "2D/Geometry.hpp"

using namespace std;

enum KeyType : unsigned char {Up = 1, Down = 2, Left = 4, Right = 8, Fullscreen = 16};

void Main::onEvent() {
    int code;
    switch (event.type) {
        case SDL_QUIT:
            quit();
            break;
        case SDL_WINDOWEVENT:
            window->onWindowEvent(event.window);
            break;
        case SDL_MOUSEWHEEL:
            if (event.wheel.y > 0) width *= 0.9;
            else if (event.wheel.y < 0) width *= 1.1;

            height = width / window->ratio;
            break;
        case SDL_KEYDOWN:
            code = event.key.keysym.scancode;
            if (code == SDL_SCANCODE_W || code == SDL_SCANCODE_UP) setKeyPressed(Up, true);
            if (code == SDL_SCANCODE_S || code == SDL_SCANCODE_DOWN) setKeyPressed(Down, true);
            if (code == SDL_SCANCODE_D || code == SDL_SCANCODE_RIGHT) setKeyPressed(Right, true);
            if (code == SDL_SCANCODE_A || code == SDL_SCANCODE_LEFT) setKeyPressed(Left, true);
            if (code == SDL_SCANCODE_F11 && !isKeyPressed(Fullscreen)) {
                setKeyPressed(Fullscreen, true);
                window->toggleFullscreen();
            }
            break;
        case SDL_KEYUP:
            code = event.key.keysym.scancode;
            if (code == SDL_SCANCODE_W || code == SDL_SCANCODE_UP) setKeyPressed(Up, false);
            if (code == SDL_SCANCODE_S || code == SDL_SCANCODE_DOWN) setKeyPressed(Down, false);
            if (code == SDL_SCANCODE_D || code == SDL_SCANCODE_RIGHT) setKeyPressed(Right, false);
            if (code == SDL_SCANCODE_A || code == SDL_SCANCODE_LEFT) setKeyPressed(Left, false);
            if (code == SDL_SCANCODE_F11 && isKeyPressed(Fullscreen)) setKeyPressed(Fullscreen, false);
            break;
    }
}

bool Main::isKeyPressed(const unsigned char& bit) {
    return pressedKeys & bit;
}

void Main::setKeyPressed(const unsigned char& bit, const bool& value) {
    if (value) pressedKeys |= bit;
    else if (!value && pressedKeys & bit) pressedKeys -= bit;
}

void Main::onLoop() {
    if (saveState == Started) {
        pfd::save_file dialog("Als Bild speichern", "Mandelbrot", {"Portable Network Graphics (PNG)", "*.png"});
        filename = dialog.result();

        if (filename.empty()) saveState = None;
        else {
            if (!filename.ends_with(".png")) filename.append(".png");

            saveState = Render;
            return;
        }
    }

    window->onLoop();

    ui->onLoop();

    if (isKeyPressed(Up)) center.y += height * deltaTime;
    if (isKeyPressed(Down)) center.y -= height * deltaTime;
    if (isKeyPressed(Right)) center.x += width * deltaTime;
    if (isKeyPressed(Left)) center.x -= width * deltaTime;
}