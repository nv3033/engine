#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <string>
#include <vector>
#include <iostream>

struct GameObject {
    int ID;
    std::string type;
    std::string name;
    float coords[3] = {0, 0, 0};
};

class UIElements
{
private:
    std::vector<GameObject> gameObjects;
    GameObject selectedObject;
    bool showAddObjectPopup = false;
    bool showPrefs = false;
    int selectedType = 0;
public:
    UIElements(/* args */);
    ~UIElements();
    void init(GLFWwindow* window);
    void update_start();
    void update_end();

    void prefs_update();
    void list_update();

    std::vector<GameObject> get_objects();
    void set_objects(std::vector<GameObject> objects);
    GameObject* find_object_by_id(int id);
};

