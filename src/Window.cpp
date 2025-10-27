#include "Window.h"

Window::Window(){}

Window::~Window(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow*, int width, int height){
    glViewport(0, 0, width, height);
}

int Window::init(const int SCR_W, const int SCR_H){
    if (!glfwInit()) return -1;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        window = glfwCreateWindow(SCR_W, SCR_H, "Two Cubes (GLM+GLAD+GLFW)", nullptr, nullptr);
        if (!window) { glfwTerminate(); return -1; }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr<<"Failed to initialize GLAD\n"; return -1;
        }
    return 0;
}

GLFWwindow* Window::get_window(){
    return window;
}

void Window::check_for_stop(){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void Window::update(){
    glfwSwapBuffers(window);
    glfwPollEvents();
    check_for_stop();

    glClearColor(0.1f,0.1f,0.12f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
