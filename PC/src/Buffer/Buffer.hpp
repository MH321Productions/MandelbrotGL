#pragma once

class Buffer {
    private:
        unsigned int vertexId, uvId, indexId;

        Buffer(const unsigned int& vertexId, const unsigned int& uvId, const unsigned int& indexId);
    
    public:
        Buffer() : vertexId(0), uvId(0), indexId(0) {}
        ~Buffer();

        void bind();
        bool isValid();
        void render();

        static Buffer* loadBuffer();
};