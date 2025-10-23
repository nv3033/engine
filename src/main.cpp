// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgramm.h"
#include "Cube.h"

#include <iostream>

const int SCR_W = 800;
const int SCR_H = 600;

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H, "Two Cubes (GLM+GLAD+GLFW)", nullptr, nullptr);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr<<"Failed to initialize GLAD\n"; return -1;
    }

    glEnable(GL_DEPTH_TEST);

    const char* vsSrc = R"glsl(
        #version 330 core
        layout (location=0) in vec3 aPos;
        layout (location=1) in vec3 aColor;
        out vec3 vColor;
        uniform mat4 uMVP;
        void main() {
            vColor = aColor;
            gl_Position = uMVP * vec4(aPos, 1.0);
        }
    )glsl";

    const char* fsSrc = R"glsl(
        #version 330 core
        in vec3 vColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vColor, 1.0);
        }
    )glsl";

    ShaderProgramm shader;
    std::string err;
    if (!shader.createFromSources(vsSrc, fsSrc, err)) {
        std::cerr << "Shader error: " << err << "\n";
        return -1;
    }

    Cube cube;
    cube.init();

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f,0.1f,0.12f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
