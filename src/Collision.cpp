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

    float currentCameraSpeed = cameraSpeed * deltaTime;

    // Move camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += currentCameraSpeed * cameraFront; // Forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= currentCameraSpeed * cameraFront; // Backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * currentCameraSpeed; // Left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * currentCameraSpeed; // Right

    // Rotate camera
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        yaw -= 1.0f; // Rotate left
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        yaw += 1.0f; // Rotate right
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        pitch += 1.0f; // Rotate up
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        pitch -= 1.0f; // Rotate down

    // Constrain pitch to avoid flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Calculate the new camera front based on yaw and pitch
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

bool Collision::checkCollision(const glm::vec3& minB, const glm::vec3& maxB) {
    return (cameraMinBounds.x <= maxB.x && cameraMaxBounds.x >= minB.x) &&
        (cameraMinBounds.y <= maxB.y && cameraMaxBounds.y >= minB.y) &&
        (cameraMinBounds.z <= maxB.z && cameraMaxBounds.z >= minB.z);
}

glm::mat4 Collision::set_view(){
    cameraPos.y = 0;
    return glm::lookAt(cameraPos,               // Позиция камеры
        cameraPos + cameraFront, // Точка, на которую смотрит камера
        cameraUp);               // Вектор "вверх"

}

void Collision::collide(glm::vec3 cubeMinBounds, glm::vec3 cubeMaxBounds){
        if (checkCollision(cubeMinBounds, cubeMaxBounds)) {
            // Calculate the center of the camera and the object
            glm::vec3 cameraCenter = (cameraMinBounds + cameraMaxBounds) * 0.5f;
            glm::vec3 objectCenter = (cubeMinBounds + cubeMaxBounds) * 0.5f;

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

glm::vec3 Collision::getCameraPos(){
    return cameraPos;
}
glm::vec3 Collision::getCameraFront(){
    return cameraFront;
}