#include <iostream>
#include <GL/glew.h>
#include <vector>

#include "Buffer.hpp"

using namespace std;

enum IdType {None = -1, Vertex, UV, Index};

static const vector<float> vertexData {
    -1.0f, 1.0f,
    1.0f, 1.0f, 
    -1.0f, -1.0f, 
    1.0f, -1.0f
};

static const vector<float> uvData {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f
};

static const vector<unsigned int> indexData {
    0, 1, 2,
    1, 2, 3 //Richtig ist: 1, 2, 3, Falsch (zum Testen): 2, 3, 0
    //2, 3, 0
};

Buffer* Buffer::loadBuffer() {
    //Buffer erstellen
    unsigned int id[3];
    glCreateBuffers(3, id);

    //Vertex Buffer erstellen
    glBindBuffer(GL_ARRAY_BUFFER, id[Vertex]);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    //UV Buffer erstellen
    glBindBuffer(GL_ARRAY_BUFFER, id[UV]);
    glBufferData(GL_ARRAY_BUFFER, uvData.size() * sizeof(float), uvData.data(), GL_STATIC_DRAW);

    //Index Buffer erstellen
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id[Index]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(unsigned int), indexData.data(), GL_STATIC_DRAW);

    return new Buffer(id[Vertex], id[UV], id[Index]);
}