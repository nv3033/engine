#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Cube {
public:
    Cube();
    ~Cube();
    
    void init();
    void draw(const glm::mat4& view, const glm::mat4& projection);
    void setPosition(const glm::vec3& position);
    
private:
    GLuint VAO, VBO, shaderProgram;
    glm::vec3 position;
    void setupShaders();
};

#endif // CUBE_H