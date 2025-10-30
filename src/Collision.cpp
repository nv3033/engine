#include "Collision.h"

Collision::Collision(/* args */)
{
}

Collision::~Collision()
{
}

// --- Функция для обработки пользовательского ввода ---
void Collision::processInput(GLFWwindow *window, float cameraSpeed, float deltaTime, glm::vec3 cubeMinBounds, glm::vec3 cubeMaxBounds) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Умножаем скорость на deltaTime, чтобы движение было равномерным независимо от FPS
    float currentCameraSpeed = cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += currentCameraSpeed * cameraFront; // Вперед
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= currentCameraSpeed * cameraFront; // Назад
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        // Влево: вычисляем "правый" вектор (cross product) и движемся в противоположном направлении
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * currentCameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        // Вправо: вычисляем "правый" вектор и движемся в его направлении
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * currentCameraSpeed;
}

bool Collision::checkCollision(const glm::vec3& minB, const glm::vec3& maxB) {
    return (cameraMinBounds.x <= maxB.x && cameraMaxBounds.x >= minB.x) &&
        (cameraMinBounds.y <= maxB.y && cameraMaxBounds.y >= minB.y) &&
        (cameraMinBounds.z <= maxB.z && cameraMaxBounds.z >= minB.z);
}

glm::mat4 Collision::set_view(){
    return glm::lookAt(cameraPos,               // Позиция камеры
        cameraPos + cameraFront, // Точка, на которую смотрит камера
        cameraUp);               // Вектор "вверх"

}

void Collision::collide(glm::vec3 cubeMinBounds, glm::vec3 cubeMaxBounds){
    glm::vec3 objectMinBounds(-0.5f, -0.5f, -0.5f);
        glm::vec3 objectMaxBounds(0.5f, 0.5f, 0.5f);
        if (checkCollision(cubeMinBounds, cubeMaxBounds)) {
            // Calculate the center of the camera and the object
            glm::vec3 cameraCenter = (cameraMinBounds + cameraMaxBounds) * 0.5f;
            glm::vec3 objectCenter = (objectMinBounds + objectMaxBounds) * 0.5f;

            // Calculate the distance vector from the object to the camera
            glm::vec3 distance = cameraCenter - objectCenter;

            // Find the smallest axis of penetration
            glm::vec3 absDistance = glm::abs(distance);
            glm::vec3 penetration; // How much to push the camera out

            if (absDistance.x < absDistance.y && absDistance.x < absDistance.z) {
                // Push the camera along the x-axis
                penetration.x = (cameraSize - absDistance.x) * (distance.x < 0 ? -1 : 1);
                cameraPos.x += penetration.x;
            } else if (absDistance.y < absDistance.x && absDistance.y < absDistance.z) {
                // Push the camera along the y-axis
                penetration.y = (cameraSize - absDistance.y) * (distance.y < 0 ? -1 : 1);
                cameraPos.y += penetration.y;
            } else {
                // Push the camera along the z-axis
                penetration.z = (cameraSize - absDistance.z) * (distance.z < 0 ? -1 : 1);
                cameraPos.z += penetration.z;
            }
        }

    cameraMinBounds = cameraPos - glm::vec3(cameraSize);
    cameraMaxBounds = cameraPos + glm::vec3(cameraSize);
}