#include "Cube.h"

Cube::Cube() : VAO(0), VBO(0) {}

Cube::~Cube() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

void Cube::init() {
    setupBuffers();
}

void Cube::setupBuffers() {
    // 36 вершин: position (3) + color (3)
    static const float vertices[] = {
        -0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,
         0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,
         0.5f, 0.5f,-0.5f,   1.0f,0.0f,0.0f,
         0.5f, 0.5f,-0.5f,   1.0f,0.0f,0.0f,
        -0.5f, 0.5f,-0.5f,   1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,   1.0f,0.0f,0.0f,

        -0.5f,-0.5f, 0.5f,   0.0f,1.0f,0.0f,
         0.5f,-0.5f, 0.5f,   0.0f,1.0f,0.0f,
         0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,
         0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,
        -0.5f, 0.5f, 0.5f,   0.0f,1.0f,0.0f,
        -0.5f,-0.5f, 0.5f,   0.0f,1.0f,0.0f,

        -0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f,0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,   0.0f,0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,   0.0f,0.0f,1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f,0.0f,1.0f,

         0.5f, 0.5f, 0.5f,   1.0f,1.0f,0.0f,
         0.5f, 0.5f,-0.5f,   1.0f,1.0f,0.0f,
         0.5f,-0.5f,-0.5f,   1.0f,1.0f,0.0f,
         0.5f,-0.5f,-0.5f,   1.0f,1.0f,0.0f,
         0.5f,-0.5f, 0.5f,   1.0f,1.0f,0.0f,
         0.5f, 0.5f, 0.5f,   1.0f,1.0f,0.0f,

        -0.5f,-0.5f,-0.5f,   1.0f,0.0f,1.0f,
         0.5f,-0.5f,-0.5f,   1.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.5f,   1.0f,0.0f,1.0f,
         0.5f,-0.5f, 0.5f,   1.0f,0.0f,1.0f,
        -0.5f,-0.5f, 0.5f,   1.0f,0.0f,1.0f,
        -0.5f,-0.5f,-0.5f,   1.0f,0.0f,1.0f,

        -0.5f, 0.5f,-0.5f,   0.0f,1.0f,1.0f,
         0.5f, 0.5f,-0.5f,   0.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,   0.0f,1.0f,1.0f,
         0.5f, 0.5f, 0.5f,   0.0f,1.0f,1.0f,
        -0.5f, 0.5f, 0.5f,   0.0f,1.0f,1.0f,
        -0.5f, 0.5f,-0.5f,   0.0f,1.0f,1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Cube::draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
