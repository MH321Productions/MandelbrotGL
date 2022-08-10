#include <iostream>
#include <GL/glew.h>
#include <SDL.h>
#include <stb_image_write.h>
#include <portable-file-dialogs.h>

#include "App.hpp"
#include "Window/Window.hpp"
#include "Shader/Shader.hpp"
#include "Buffer/Buffer.hpp"
#include "2D/Renderer.hpp"
#include "UI/Overlay.hpp"

using namespace std;

void Main::onRender() {
    //Letztes Bild löschen
    glClear(GL_COLOR_BUFFER_BIT);
    renderer->setColor(Colors::Transparent);
    renderer->clear();

    //Rendern Hier
    mainShader->bind();
    mainShader->setMandelbrotValues(window, center.x, center.y, width, ui->colorMap);
    buffer->render();
    ui->onRender();

    if (saveState == Render) saveImage();

    //Rendern abschließen
    overlayShader->bind();
    renderer->render(overlayShader->getUniformLocation("textureSampler"), buffer);
    window->onRender();
}

void Main::saveImage() {
    saveState = None;

    //Back Buffer auslesen
    glReadBuffer(GL_BACK);

    //Pixeldaten in Array schreiben
    unsigned char* data = new unsigned char[window->width * window->height * 3];
    glReadPixels(0, 0, window->width, window->height, GL_RGB, GL_UNSIGNED_BYTE, data);

    //Pixeldaten speichern
    stbi_flip_vertically_on_write(true);
    int result = stbi_write_png(filename.c_str(), window->width, window->height, 3, data, window->width * 3);

    //Daten löschen
    delete[] data;

    //Ergebnis anzeigen
    if (result) pfd::notify("Speichern erfolgreich", "Das Bild wurde erfolgreich gespeichert");
    else pfd::notify("Fehler", "Das Bild konnte nicht gespeichert werden", pfd::icon::error);
}