// main.cpp
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Колбэк изменения размера окна
void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
}

bool checkCompileErrors(GLuint shader, const char* type) {
    GLint success;
    GLchar infoLog[1024];
    if (std::string(type) != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
            return false;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
            return false;
        }
    }
    return true;
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const int SCR_W = 800, SCR_H = 600;
    GLFWwindow* window = glfwCreateWindow(SCR_W,SCR_H,"Cube (GLM+GLAD+GLFW)",nullptr,nullptr);
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

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vsSrc,nullptr);
    glCompileShader(vs);
    if(!checkCompileErrors(vs,"VERTEX")) return -1;

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fsSrc,nullptr);
    glCompileShader(fs);
    if(!checkCompileErrors(fs,"FRAGMENT")) return -1;

    GLuint prog = glCreateProgram();
    glAttachShader(prog,vs); glAttachShader(prog,fs);
    glLinkProgram(prog);
    if(!checkCompileErrors(prog,"PROGRAM")) return -1;
    glDeleteShader(vs); glDeleteShader(fs);

    float vertices[] = {
        // positions         // colors
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

    GLuint VBO, VAO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLint uMVP = glGetUniformLocation(prog, "uMVP");

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f,0.1f,0.12f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float t = (float)glfwGetTime();
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,0.0f,-3.0f));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), t * 0.8f, glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model, t * 0.4f, glm::vec3(1.0f,0.0f,0.0f));
        glm::mat4 mvp = proj * view * model;

        glUseProgram(prog);
        glUniformMatrix4fv(uMVP, 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteProgram(prog);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
