
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

#include <map>





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





// --- отрисовка стены ---
void drawWall(UIElements& ui_elements, 
    Collision& collision, 
    Wall& myWall, 
    ShaderProgramm& shader,
    GLFWwindow* window,
    glm::mat4& view, glm::mat4& projection,
    int index) {
    collision.collide(myWall.get_bounds(0), myWall.get_bounds(1));

    //создаем матрицу модели и указываем позицию
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(
        ui_elements.get_objects()[index].coords[0],
        ui_elements.get_objects()[index].coords[1],
        ui_elements.get_objects()[index].coords[2]
    ));

    //определяем вращение
    float angleX = glm::radians(ui_elements.get_objects()[index].rotation[0]);
    float angleY = glm::radians(ui_elements.get_objects()[index].rotation[1]);
    float angleZ = glm::radians(ui_elements.get_objects()[index].rotation[2]);

    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 rotationMatrix = rotationZ * rotationY * rotationX;
    model = model * rotationMatrix;

    //загружаем нужные матрицы в шейдер
    shader.upload_matrix(model, view, projection);
    
    //переустанавливаем граница коллизии
    myWall.move_bounds(ui_elements.get_objects()[index].coords);
    myWall.rotate_bounds(ui_elements.get_objects()[index].rotation);

    //ограничиваем движение в пространство коллизии
    collision.collide(myWall.get_bounds(0), myWall.get_bounds(1));
    collision.processInput(window, cameraSpeed / ui_elements.get_objects().size(), deltaTime, myWall.get_bounds(0), myWall.get_bounds(1));

    //рисуем стену
    myWall.draw();
}




// --- отрисовка врага ---
void drawEnemy(UIElements& ui_elements, 
        Collision& collision, 
        Enemy& enemy, 
        ShaderProgramm& shader, 
        GLFWwindow* window, 
        glm::mat4& view, glm::mat4 projection, 
        int index){

    glm::vec3 objectPosition = glm::vec3(
        ui_elements.get_objects()[index].coords[0],
        ui_elements.get_objects()[index].coords[1],
        ui_elements.get_objects()[index].coords[2]);

    glm::vec3 cameraPosition = collision.getCameraPos();
    glm::vec3 directionToCamera = glm::normalize(cameraPosition - objectPosition);
    glm::mat4 rotation = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), directionToCamera, glm::vec3(0.0f, -1.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f); 
    model = glm::translate(model, objectPosition);
    model = model * rotation;;

    shader.upload_matrix(model, view, projection);
    //collision.collide(myWall.get_bounds(0), myWall.get_bounds(1));
    collision.processInput(window, cameraSpeed/ui_elements.get_objects().size(), deltaTime, enemy.get_bounds(0), enemy.get_bounds(1));

    enemy.draw();
}





// --- Основная функция ---
int main() {
    //Инициализация GLFW и создание окна
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

    //Инициализация GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Включаем тест глубины для корректного отображения 3D объектов
    glEnable(GL_DEPTH_TEST);

    //подклюючаем основные обьекты
    Collision collision;

    UIElements ui_elements;
    ui_elements.init(window);

    Wall myWall;
    myWall.init();

    Enemy enemy;
    enemy.init();

    Textures textures;
    textures.init();

    ShaderProgramm shader;
    shader.init();

    //Основной цикл рендеринга
    while (!glfwWindowShouldClose(window)) {
        // Вычисление deltaTime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Очистка буферов цвета и глубины
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //обновляем окна интерфейса
        ui_elements.update_start();
        ui_elements.list_update();
        ui_elements.top_bar_update(window);
        ui_elements.prefs_update(sizeof(textures.get_textures()));

        // Создание матриц Model, View, Projection

        glm::mat4 view = collision.set_view();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        //создаем map для отпределения типа текущего элемента
        size_t typesSize = sizeof(ui_elements.availableTypes) / sizeof(ui_elements.availableTypes[0]);
        std::map<std::string, int> typesMap;

        for(int i = 0; i < typesSize; i ++){ typesMap[ui_elements.availableTypes[i]] = i; }        

        //рисуем обьекты игры
        for (int i = 0; i < ui_elements.get_objects().size(); i ++){
            //активируем нужную текстуру
            textures.activate(ui_elements.get_objects()[i].texture_id);
            
            //определяем тип текущего обьекта
            switch (typesMap[ui_elements.get_objects()[i].type])
            {
            case 0:
                break;
            case 1:
                drawWall(ui_elements, collision, myWall, shader, window, view, projection, i);
                break;
            case 2:
                drawEnemy(ui_elements, collision, enemy, shader, window, view, projection, i);
                break;
            default:
                break;
            }
        }
        
        //отрисовка окон
        ui_elements.update_end();

        // Обмен буферов и опрос событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //очистка памяти
    shader.~ShaderProgramm();
    textures.~Textures();
    ui_elements.~UIElements();

    //закрываем окно
    glfwTerminate();
    return 0;
}
