
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "Textures.h"
#include "ShaderProgramm.h"
#include "Basic_Objects.h"
#include "UIElements.h"

// --- Глобальные настройки окна ---
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// --- Переменные для камеры ---
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);   // Начальная позиция камеры
float cameraSize = 0.5f; // Half the size for AABB
glm::vec3 cameraMinBounds = cameraPos - glm::vec3(cameraSize);
glm::vec3 cameraMaxBounds = cameraPos + glm::vec3(cameraSize);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);  // Вектор, куда смотрит камера (на -Z)
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);   // Вектор "вверх" для камеры

float cameraSpeed = 2.5f; // Скорость движения камеры (единиц в секунду)
float deltaTime = 0.0f;   // Время, прошедшее между текущим и предыдущим кадром
float lastFrame = 0.0f;   // Время последнего кадра

// --- Функции обратного вызова ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// --- Функция для обработки пользовательского ввода ---
void processInput(GLFWwindow *window) {
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

bool checkCollision(const glm::vec3& minA, const glm::vec3& maxA,
    const glm::vec3& minB, const glm::vec3& maxB) {
    return (minA.x <= maxB.x && maxA.x >= minB.x) &&
        (minA.y <= maxB.y && maxA.y >= minB.y) &&
        (minA.z <= maxB.z && maxA.z >= minB.z);
}

// --- Основная функция ---
int main() {
    // 1. Инициализация GLFW и создание окна
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Textured Cube with Camera", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 2. Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Включаем тест глубины для корректного отображения 3D объектов
    glEnable(GL_DEPTH_TEST);

    UIElements ui_elements;
    ui_elements.init(window);

    // Создаем и инициализируем куб
    Cube myCube;
    myCube.init();
    Wall myWall;
    myWall.init();

    Textures textures;
    //texture.set_path("/home/anton/Desktop/portfolio/texture_test/img/stone_tex.png");

    textures.init();

    ShaderProgramm shader;
    shader.init();

    // 6. Основной цикл рендеринга
    while (!glfwWindowShouldClose(window)) {
        // Вычисление deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Обработка пользовательского ввода (движение камеры)
        processInput(window);

        // Очистка буферов цвета и глубины
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ui_elements.update_start();
        ui_elements.list_update();
        ui_elements.prefs_update(sizeof(textures.get_textures()));


        // Создание матриц Model, View, Projection

        glm::mat4 view = glm::lookAt(cameraPos,               // Позиция камеры
                                     cameraPos + cameraFront, // Точка, на которую смотрит камера
                                     cameraUp);               // Вектор "вверх"

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        for (int i = 0; i < ui_elements.get_objects().size(); i ++){
            
            textures.activate(ui_elements.get_objects()[i].texture_id);

            glm::vec3 objectMinBounds(-0.5f, -0.5f, -0.5f);
            glm::vec3 objectMaxBounds(0.5f, 0.5f, 0.5f);
            if (checkCollision(cameraMinBounds, cameraMaxBounds, myCube.get_bounds(0), myCube.get_bounds(1))) {
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

            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ui_elements.get_objects()[i].coords[0], 
                ui_elements.get_objects()[i].coords[1], 
                ui_elements.get_objects()[i].coords[2]));


            float angleX = glm::radians(ui_elements.get_objects()[i].rotation[0]); // Rotate 30 degrees around X-axis
            float angleY = glm::radians(ui_elements.get_objects()[i].rotation[1]); // Rotate 45 degrees around Y-axis
            float angleZ = glm::radians(ui_elements.get_objects()[i].rotation[2]); // Rotate 60 degrees around Z-axis

            glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
            glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
            glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis
            
            glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX; // Apply Z-Y-X rotation

            model = model * rotationMatrix;     // Apply rotation


            shader.upload_matrix(model, view, projection);
            if (ui_elements.get_objects()[i].type == "cube")
                myCube.draw();
            else if (ui_elements.get_objects()[i].type == "wall")
                myWall.draw();
        }
        
        ui_elements.update_end();
        // Обмен буферов и опрос событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    myCube.destroy();
    shader.~ShaderProgramm();
    textures.~Textures();
    ui_elements.~UIElements();

    glfwTerminate();
    return 0;
}
