#pragma once

//#include <SDL_video.h>

class Main;
struct SDL_WindowEvent;
struct SDL_Surface;
struct SDL_Window;
using SDL_GLContext = void*;

class Window {
    private:
        SDL_Window* window;
        SDL_GLContext context;
        SDL_Surface* icon;
        Main* main;
        unsigned int vao;
        bool lastLeftPressed = false;
        bool currentLeftPressed = false;

        bool isFullscreen = false;

        void onCleanup();
    
    public:
        int width = 1280, height = 720; //Wird mit HD initialisiert
        int mouseOffsetX = 0, mouseOffsetY = 0; //Der Offset der Maus zum Anfangspunkt (beim Verschieben) in Pixeln
        int mouseX, mouseY;
        float ratio;

        Window(Main* main);
        ~Window();

        bool onInit();
        void onLoop();
        void onWindowEvent(const SDL_WindowEvent& event);
        void onRender();
        bool isClicked();
        bool isPressed();
        void toggleFullscreen();
};