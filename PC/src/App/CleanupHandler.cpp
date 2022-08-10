#include <iostream>
#include <SDL.h>

#include "App.hpp"
#include "Window/Window.hpp"
#include "Shader/Shader.hpp"
#include "Buffer/Buffer.hpp"
#include "2D/Renderer.hpp"
#include "UI/Overlay.hpp"
#include "2D/Font.hpp"
#include "Texture/Texture.hpp"

using namespace std;

void Main::onCleanup() {
    
    delete ui;
    delete mainShader;
    delete overlayShader;
    delete buffer;
    delete textureMan;
    delete renderer;
    delete fontMan;
    
    delete window;

    SDL_Quit();
}