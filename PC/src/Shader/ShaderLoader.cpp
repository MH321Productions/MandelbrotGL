#include <iostream>
#include <GL/glew.h>

#include "Shader.hpp"
#include "ShaderCode.hpp"

using namespace std;

unsigned int compileShader(const unsigned int& type, const string& src);

Shader* Shader::loadMainShader() {
    return loadShader(ShaderCode::MandelbrotVertex, ShaderCode::MandelbrotFragment);
}

Shader* Shader::loadOverlayShader() {
    return loadShader(ShaderCode::OverlayVertex, ShaderCode::OverlayFragment);
}

Shader* Shader::loadShader(const string& vertexCode, const string& fragmentCode) {
    unsigned int vertexID = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragmentID = compileShader(GL_FRAGMENT_SHADER, fragmentCode);

    if (vertexID == 0 || fragmentID == 0) {
        cerr << "Konnte den Shader nicht erstellen, da eine (oder beide) Komponenten nicht kompiliert werden konnten" << endl;
        return NULL;
    }

    unsigned int program = glCreateProgram();

    glAttachShader(program, vertexID);
    glAttachShader(program, fragmentID);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexID);
    glDeleteShader(fragmentID);

    return new Shader(program);
}

unsigned int compileShader(const unsigned int& type, const string& src) {
    unsigned int id = glCreateShader(type);
    const char* cstr = src.c_str();
    glShaderSource(id, 1, &cstr, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length);
        glGetShaderInfoLog(id, length, &length, message);
        
        cout << "Konnte " << (type == GL_VERTEX_SHADER ? "Vertexshader" : "Fragmentshader")<< " nicht kompilieren" << endl;
        cout << message << endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}