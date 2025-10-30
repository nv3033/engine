#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class Collision
{
private:
    // --- Переменные для камеры ---
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);   // Начальная позиция камеры
    float cameraSize = 0.5f; // Half the size for AABB
    glm::vec3 cameraMinBounds = cameraPos - glm::vec3(cameraSize);
    glm::vec3 cameraMaxBounds = cameraPos + glm::vec3(cameraSize);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // Вектор, куда смотрит камера (на -Z)
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);   // Вектор "вверх" для камеры
public:
    Collision(/* args */);
    ~Collision();

    // --- Функция для обработки пользовательского ввода ---
    void processInput(GLFWwindow *window, float cameraSpeed, float deltaTime, glm::vec3 cubeMinBounds, glm::vec3 cubeMaxBounds);
    bool checkCollision(const glm::vec3& minB, const glm::vec3& maxB);
    glm::mat4 set_view();
    void collide(glm::vec3 cubeMinBounds, glm::vec3 cubeMaxBounds);

};


