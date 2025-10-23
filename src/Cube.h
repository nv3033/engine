#pragma once
#include <glad/glad.h>

class Cube {
public:
    Cube();
    ~Cube();

    void init();
    void draw() const;

private:
    GLuint VAO, VBO;
    void setupBuffers();
};
