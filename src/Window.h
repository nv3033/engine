#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
private:
    GLFWwindow* window;
public:

    Window();
    ~Window();
    int init(const int SCR_W, const int SCR_H);
    void check_for_stop();
    void update();
    GLFWwindow* get_window();
};
