
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
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ui_elements.get_objects()[i].coords[0], 
                ui_elements.get_objects()[i].coords[1], 
                ui_elements.get_objects()[i].coords[2]));
            shader.upload_matrix(model, view, projection);
            myCube.draw();
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
