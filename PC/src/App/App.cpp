#include <iostream>
#include <thread>
#include <chrono>

#include "App.hpp"
#include "Window/Window.hpp"

using namespace std;
using namespace chrono;

time_point<system_clock> start, ende;
duration<double> dauer;

int Main::onExecute() {
    if (running) {
        cerr << "Die Anwendung läuft bereits" << endl;
        return 1;
    }

    running = true;
    deltaTime = 0.0;

    if (!onInit()) {
        cerr << "Die Anwendung konnte nicht initialisiert werden. Breche Ab" << endl;
        return 1;
    }

    while (running) { //Main Loop
        calculateDeltaTime(true);

        while (SDL_PollEvent(&event)) onEvent();

        onLoop();
        onRender();

        calculateDeltaTime(false);

        this_thread::sleep_for(milliseconds(10));
    }

    onCleanup();

    return 0;
}

void Main::calculateDeltaTime(const bool& startFrame) {
    if (startFrame) {
        start = system_clock::now();
    } else {
        ende = system_clock::now();
        dauer = ende - start;
        deltaTime = dauer.count();
        fps = 1 / deltaTime;
    }
}

void Main::quit() {
    running = false;
}

void Main::resetCoords() {
    center = {-0.75f, 0.0f};
    width = 2.1;
    height = width / window->ratio;
}

void Main::startSave() {
    if (!saveState) saveState = Started;
}