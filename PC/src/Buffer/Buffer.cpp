#include <iostream>
#include <GL/glew.h>

#include "Buffer.hpp"

using namespace std;

Buffer::Buffer(const unsigned int& vertexId, const unsigned int& uvId, const unsigned int& indexId)
: vertexId(vertexId), uvId(uvId), indexId(indexId) {
    cout << "Buffer " << vertexId << ", " << uvId << " und " << indexId << " erstellt" << endl;
}

Buffer::~Buffer() {
    cout << "Buffer " << vertexId << ", " << uvId << " und " << indexId << " gelöscht" << endl;

    if (vertexId != 0) glDeleteBuffers(1, &vertexId);
    if (uvId != 0) glDeleteBuffers(1, &uvId);
    if (indexId != 0) glDeleteBuffers(1, &indexId);
}

void Buffer::bind() {
    if (vertexId != 0) {
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexId);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    if (uvId != 0) {
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvId);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    if (indexId != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexId);
    }
}

bool Buffer::isValid() {
    return vertexId && uvId && indexId;
}

void Buffer::render() {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
}