#include <iostream>
#include <GL/glew.h>

#include "Shader.hpp"
#include "Window/Window.hpp"

using namespace std;

void Shader::bind() {
    if (id != 0) glUseProgram(id);
}

Shader::Shader(const unsigned int& id) : id(id) {
    cout << "Shader " << id << " erstellt" << endl;
}

Shader::~Shader() {
    cout << "Shader " << id << " gelöscht" << endl;
    if (id != 0) glDeleteProgram(id);
}

int Shader::getUniformLocation(const string& name) {
    if (id == 0) return 0;
    return glGetUniformLocation(id, name.c_str());
}

void Shader::setMandelbrotValues(Window* window, const float& centerX, const float& centerY, const float& width, const int& colorMap) {
    int sizeLoc = getUniformLocation("windowSize");
    int centerLoc = getUniformLocation("centerWidthHeight");
    int colorLoc = getUniformLocation("colorMap");

    glUniform2f(sizeLoc, window->width, window->height);
    glUniform4f(centerLoc, centerX, centerY, width, width / window->ratio);
    glUniform1i(colorLoc, colorMap);
}