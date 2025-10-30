
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
#include "Collision.h"

// --- Глобальные настройки окна ---
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float cameraSpeed = 2.5f; // Скорость движения камеры (единиц в секунду)
float deltaTime = 0.0f;   // Время, прошедшее между текущим и предыдущим кадром
float lastFrame = 0.0f;   // Время последнего кадра

// --- Функции обратного вызова ---
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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

    Collision collision;

    UIElements ui_elements;
    ui_elements.init(window);

    
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

        // Очистка буферов цвета и глубины
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ui_elements.update_start();
        ui_elements.list_update();
        ui_elements.prefs_update(sizeof(textures.get_textures()));

        // Создание матриц Model, View, Projection

        glm::mat4 view = collision.set_view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        for (int i = 0; i < ui_elements.get_objects().size(); i ++){
            textures.activate(ui_elements.get_objects()[i].texture_id);

            collision.collide(myWall.get_bounds(0), myWall.get_bounds(1));
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
            if (ui_elements.get_objects()[i].type == "wall"){

                myWall.move_bounds(ui_elements.get_objects()[i].coords);
                myWall.rotate_bounds(ui_elements.get_objects()[i].rotation);
                
                collision.collide(myWall.get_bounds(0), myWall.get_bounds(1));
                collision.processInput(window, cameraSpeed/ui_elements.get_objects().size(), deltaTime, myWall.get_bounds(0), myWall.get_bounds(1));
                if (myWall.isCameraLookingAt(collision.getCameraPos(), collision.getCameraFront()))
                    std::cout << "yes" << std::endl;
                else std::cout << "no" << std::endl;

                myWall.draw();
            }
        }
        
        ui_elements.update_end();
        // Обмен буферов и опрос событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    shader.~ShaderProgramm();
    textures.~Textures();
    ui_elements.~UIElements();

    glfwTerminate();
    return 0;
}
