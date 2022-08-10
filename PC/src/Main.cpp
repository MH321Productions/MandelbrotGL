#include <iostream>

#undef SDL_MAIN_HANDLED
#include <SDL_main.h>

#include "App/App.hpp"

using namespace std;

int main(int anzahl, char* args[]) {
    cout << "Hello World" << endl;

    Main* app = new Main;
    int result = app->onExecute();

    delete app;

    return result;
}