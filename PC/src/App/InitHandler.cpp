#include <iostream>
#include <GL/glew.h>
#include <SDL.h>

#include "App.hpp"
#include "Window/Window.hpp"
#include "Shader/Shader.hpp"
#include "Buffer/Buffer.hpp"
#include "2D/Font.hpp"
#include "2D/Renderer.hpp"
#include "Texture/Texture.hpp"
#include "UI/Overlay.hpp"

using namespace std;

bool Main::onInit() {
    window = new Window(this);
    if (!window->onInit()) return false;

    //Shader laden
    cout << "Lade Shader" << endl;
    mainShader = Shader::loadMainShader();
    overlayShader = Shader::loadOverlayShader();
    if (!mainShader) {
        cerr << "Konnte Mandelbrot-Shader nicht laden. Breche ab" << endl;
        return false;
    } else if (!overlayShader) {
        cerr << "Konnte Texturshader nicht laden. Breche ab" << endl;
        return false;
    }

    //Buffer laden
    cout << "Lade Buffer" << endl;
    buffer = Buffer::loadBuffer();
    if (!buffer->isValid()) {
        cerr << "Die Buffer konnten nicht geladen werden. Breche ab" << endl;
        return false;
    }
    buffer->bind();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    fontMan = new FontManager;
    renderer = new Renderer(window->width, window->height, this);
    textureMan = new TextureManager;
    ui = new Overlay(this);

    center = {-0.75f, 0.0f};
    width = 2.1;
    height = width / window->ratio;

    return true;
}