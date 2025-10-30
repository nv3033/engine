#include "Basic_Objects.h"
// #include "Cube.h" // Больше не нужен, Cube теперь в своем файле

Basic_Object::Basic_Object() : VAO(0), VBO(0) {}

Basic_Object::~Basic_Object() {
    destroy(); // Вызываем destroy при уничтожении базового объекта
}

void Basic_Object::destroy() {
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}

void Basic_Object::init() {
    // Базовая реализация может быть пустой
}

void Basic_Object::draw() const {
    // Базовая реализация может быть пустой
}

// Удаляем Cube::init и Cube::destroy, если они были здесь
// void Cube::init(){ ... }
// void Cube::destroy(){ ... }
Cube::Cube() : Basic_Object() {
    // Конструктор базового класса уже инициализирует VAO/VBO в 0
}

Cube::~Cube() {
    // Деструктор базового класса вызовет destroy()
}

void Cube::init() {
    // Данные вершин для куба (позиции и текстурные координаты)
    objectMinBounds = objectStartMinBounds;
    objectMaxBounds = objectStartMaxBounds;
    float vertices[] = {
        // positions          // texture coords
        // Передняя грань
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // Задняя грань
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // Левая грань
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        // Правая грань
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        // Нижняя грань
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        // Верхняя грань
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Атрибут позиции (layout (location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Атрибут текстурных координат (layout (location = 1))
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::draw() const {
    if (VAO == 0) {
        std::cerr << "Error: Cube VAO is not initialized!" << std::endl;
        return;
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); // Куб состоит из 36 вершин
    glBindVertexArray(0);
}

glm::vec3 Cube::get_bounds(int index){
    if (index == 0)
        return objectMinBounds;
    if (index == 1)
        return objectMaxBounds;
}

void Cube::move_bounds(float coords[3]){
    objectMinBounds.x = objectStartMinBounds[0] + coords[0]; 
    objectMinBounds.y = objectStartMinBounds[1] + coords[1]; 
    objectMinBounds.z = objectStartMinBounds[2] + coords[2]; 
    objectMaxBounds.x = objectStartMaxBounds[0] + coords[0]; 
    objectMaxBounds.y = objectStartMaxBounds[1] + coords[1]; 
    objectMaxBounds.z = objectStartMaxBounds[2] + coords[2]; 
}

Wall::Wall() : Basic_Object() {
    // Конструктор базового класса уже инициализирует VAO/VBO в 0
}

Wall::~Wall() {
    // Деструктор базового класса вызовет destroy()
}

void Wall::init() {
    // Данные вершин для куба (позиции и текстурные координаты)
    float vertices[] = {
        // positions          // texture coords
        // Передняя грань
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Атрибут позиции (layout (location = 0))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Атрибут текстурных координат (layout (location = 1))
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Wall::draw() const {
    if (VAO == 0) {
        std::cerr << "Error: Cube VAO is not initialized!" << std::endl;
        return;
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6); // Куб состоит из 36 вершин
    glBindVertexArray(0);
}

glm::vec3 Wall::get_bounds(int index){
    if (index == 0)
        return objectMinBounds;
    if (index == 1)
        return objectMaxBounds;
}

void Wall::move_bounds(float coords[3]){
    objectMinBounds.x = objectStartMinBounds[0] + coords[0]; 
    objectMinBounds.y = objectStartMinBounds[1] + coords[1]; 
    objectMinBounds.z = objectStartMinBounds[2] + coords[2]; 
    objectMaxBounds.x = objectStartMaxBounds[0] + coords[0]; 
    objectMaxBounds.y = objectStartMaxBounds[1] + coords[1]; 
    objectMaxBounds.z = objectStartMaxBounds[2] + coords[2]; 
}
void Wall::rotate_bounds(float coords[3]){
    float xRad = glm::radians(coords[0]);
    float yRad = glm::radians(coords[1]);
    float zRad = glm::radians(coords[2]);

    // Create individual rotation matrices
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), xRad, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), yRad, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), zRad, glm::vec3(0.0f, 0.0f, 1.0f));

    // Combine the rotations (Y * X * Z order is commonly used)
    glm::mat4 rotationMatrix = rotateZ * rotateY * rotateX;

    // Convert the glm::vec3 to a glm::vec4 (homogeneous coordinates)
    glm::vec4 rotatedBounds = rotationMatrix * glm::vec4(objectStartMinBounds, 1.0f);

    glm::vec3 rotatedVector = glm::vec3(rotatedBounds);

    // Output the result
    //std::cout << "Rotated Vector: (" 
    //          << rotatedVector.x << ", " 
    //          << rotatedVector.y << ", " 
    //          << rotatedVector.z << ")" << std::endl;
}

bool Wall::isCameraLookingAt(const glm::vec3& cameraPos, const glm::vec3& cameraFront) {
    // Находим центр куба
    glm::vec3 cubeCenter = (objectMinBounds + objectMaxBounds) * 0.5f;

    // Вектор от камеры к центру куба
    glm::vec3 toCube = glm::normalize(cubeCenter - cameraPos);

    // Направление камеры
    glm::vec3 cameraDirection = glm::normalize(cameraFront);

    // Рассчитываем угол между направлением камеры и вектором к кубу
    float angle = glm::degrees(acos(glm::dot(cameraDirection, toCube)));

    // Установите порог, чтобы определить, "смотрит" ли камера на куб
    float threshold = 1.0f; // Например, 45 градусов

    return angle < threshold;
}

/*Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}*/
