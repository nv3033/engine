// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgramm.h"
#include "Cube.h"
#include "Window.h"
#include "UIElements.h"

#include <iostream>

const int SCR_W = 800;
const int SCR_H = 600;

Window window;
UIElements ui_elements;

// Camera settings
float cameraSpeed = 0.05f; // Speed of camera movement
glm::vec3 cameraPos(0.0f, 0.0f, -6.0f); // Initial camera position

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos.z += cameraSpeed; // Move forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos.z -= cameraSpeed; // Move backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos.x -= cameraSpeed; // Move left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos.x += cameraSpeed; // Move right
}

int main() {
    window.init(SCR_W, SCR_H);
    glEnable(GL_DEPTH_TEST);

    ui_elements.init(window.get_window());

    ShaderProgramm shader;
    std::string err;
    if (!shader.createFromSources(err)) {
        std::cerr << "Shader error: " << err << "\n";
        return -1;
    }

    Cube cube;
    cube.init();

    while (!glfwWindowShouldClose(window.get_window())) {
        processInput(window.get_window()); // Process input for camera movement


        ui_elements.update_start();
        ui_elements.list_update();
        ui_elements.prefs_update();
        window.update();

        float t = (float)glfwGetTime();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPos); // Update view based on camera position

        shader.use();

        for (int i = 0; i < ui_elements.get_objects().size(); i ++){
            if (ui_elements.get_objects()[i].type == "cube"){
                glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
                //model = glm::rotate(model, t * 1.2f, glm::vec3(0.0f, 1.0f, 0.0f));
                //model = glm::rotate(model, t * 0.6f, glm::vec3(1.0f, 0.0f, 0.0f));
                glm::mat4 mvp = proj * view * model;
                shader.setMat4("uMVP", glm::value_ptr(mvp));
                cube.draw();
            }
        }

        // Левый куб (слева, вращается быстрее)
        /*{
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.2f, 0.0f, 0.0f));
            //model = glm::rotate(model, t * 1.2f, glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, t * 0.6f, glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 mvp = proj * view * model;
            shader.setMat4("uMVP", glm::value_ptr(mvp));
            cube.draw();
        }

        // Правый куб (справа, вращается медленнее и немного масштабирован)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.2f, 0.0f, 0.0f));
            //model = glm::rotate(model, t * 0.6f, glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::rotate(model, t * 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
            //smodel = glm::scale(model, glm::vec3(0.9f)); // немного меньше
            glm::mat4 mvp = proj * view * model;
            shader.setMat4("uMVP", glm::value_ptr(mvp));
            cube.draw();
        }*/
        ui_elements.update_end();
    }
    ui_elements.~UIElements();
    window.~Window();
    return 0;
}
