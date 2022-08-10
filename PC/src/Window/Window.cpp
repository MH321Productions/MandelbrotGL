#include <iostream>
#include <GL/glew.h>
#include <SDL.h>

#include "Window.hpp"
#include "App/App.hpp"
#include "Texture/Texture.hpp"
#include "2D/Renderer.hpp"

using namespace std;

Window::Window(Main* main) : main(main) {
    ratio = (float) width / (float) height;
}

Window::~Window() {
    onCleanup();
}

bool Window::onInit() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cerr << "Konnte SDL nicht initialisieren: " << SDL_GetError() << endl;
        return false;
    }

    //OpenGL Version 3.3 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Fenster erstellen
    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
    window = SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (window == NULL) {
        cerr << "Das Fenster konnte nicht erstellt werden: " << SDL_GetError() << endl;
        return false;
    }

    //Icon setzen
    icon = Texture::loadIcon();
    if (icon) SDL_SetWindowIcon(window, icon);

    //OpenGL Kontext erstellen
    context = SDL_GL_CreateContext(window);
    if (context == NULL) {
        cerr << "Der OpenGL Kontext konnte nicht erstellt werden: " << SDL_GetError() << endl;
        return false;
    }

    //GLEW initialisieren
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        cerr << "Konnte GLEW nicht initialisieren" << endl;
        return false;
    }

    //Dunkelblauer Hintergrund
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    //Vertex Array Object initialisieren
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    cout << endl << "OpenGL Information:" << endl;
    cout << "Version: " << glGetString(GL_VERSION) << endl;
    cout << "Grafikkartenhersteller: " << glGetString(GL_VENDOR) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;

    return true;
}

void Window::onLoop() {
    lastLeftPressed = currentLeftPressed;

    unsigned int btn = SDL_GetMouseState(&mouseX, &mouseY);
    currentLeftPressed = btn & 1;
}

void Window::onWindowEvent(const SDL_WindowEvent& event) {
    if (event.event == SDL_WINDOWEVENT_RESIZED || event.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
        width = event.data1;
        height = event.data2;
        ratio = (float) width / (float) height;

        cout << width << "x" << height << " (" << ratio << ")" << endl;
        glViewport(0, 0, width, height);
        main->renderer->resize(width, height);
    }
}

void Window::onCleanup() {
    glDeleteVertexArrays(1, &vao); //Vertex Arrays löschen
}

void Window::onRender() {
    SDL_GL_SwapWindow(window); //Rendern abschließen, Buffer tauschen
}

bool Window::isClicked() {
    return currentLeftPressed && !lastLeftPressed;
}

bool Window::isPressed() {
    return currentLeftPressed && lastLeftPressed;
}

void Window::toggleFullscreen() {
    if (isFullscreen) SDL_SetWindowFullscreen(window, 0);
    else SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    isFullscreen = !isFullscreen;
}