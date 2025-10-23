// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgramm.h"
#include "Cube.h"
#include "Window.h"

#include <iostream>

const int SCR_W = 800;
const int SCR_H = 600;

Window window;

int main() {
    window.init(SCR_W, SCR_H);
    glEnable(GL_DEPTH_TEST);

    ShaderProgramm shader;
    std::string err;
    if (!shader.createFromSources(err)) {
        std::cerr << "Shader error: " << err << "\n";
        return -1;
    }

    Cube cube;
    cube.init();

    while (!glfwWindowShouldClose(window.get_window())) {

        window.update();

        float t = (float)glfwGetTime();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f)); // отодвинем камеру чуть дальше

        shader.use();

        // Левый куб (слева, вращается быстрее)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.2f, 0.0f, 0.0f));
            model = glm::rotate(model, t * 1.2f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, t * 0.6f, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 mvp = proj * view * model;
            shader.setMat4("uMVP", glm::value_ptr(mvp));
            cube.draw();
        }

        // Правый куб (справа, вращается медленнее и немного масштабирован)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 0.0f, 0.0f));
            model = glm::rotate(model, t * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, t * 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.9f)); // немного меньше
            glm::mat4 mvp = proj * view * model;
            shader.setMat4("uMVP", glm::value_ptr(mvp));
            cube.draw();
        }
    }
    window.~Window();
    return 0;
}
