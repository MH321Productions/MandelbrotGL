#pragma once
#include <string>

class Window;

class Shader {
    private:
        unsigned int id;

        Shader(const unsigned int& id);

        static Shader* loadShader(const std::string& vertexCode, const std::string& fragmentCode);

    public:
        Shader() : id(0) {}
        ~Shader();

        void bind();
        int getUniformLocation(const std::string& name);
        void setMandelbrotValues(Window* window, const float& centerX, const float& centerY, const float& width, const int& colorMap);

        static Shader* loadOverlayShader();
        static Shader* loadMainShader();
};