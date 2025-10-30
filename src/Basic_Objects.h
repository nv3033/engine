#ifndef BASIC_OBJECTS_H
#define BASIC_OBJECTS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Basic_Object {
public:
    Basic_Object();
    virtual ~Basic_Object(); // Виртуальный деструктор для правильной очистки
    
    // Виртуальные методы, которые будут переопределены в дочерних классах
    virtual void init();
    virtual void draw() const;
    virtual void destroy();

protected:
    GLuint VAO = 0;
    GLuint VBO = 0;
    glm::vec3 objectMinBounds;
    glm::vec3 objectMaxBounds;
};

// Удаляем объявление класса Cube отсюда, если оно было
// Он будет в отдельном Cube.h
class Cube : public Basic_Object {
public:
    Cube();
    ~Cube() override; // Переопределение виртуального деструктора

    void init() override;   // Переопределение метода инициализации
    void draw() const override; // Переопределение метода отрисовки
    // destroy() будет вызвана из деструктора Basic_Object,
    // и его реализация в Basic_Object достаточна
    glm::vec3 get_bounds(int index);
    void move_bounds(float coords[3]);
    void rotate_bounds(float coords[3]);
private:
    glm::vec3 objectStartMinBounds = {-0.5f, -0.5f, 0.0f};
    glm::vec3 objectStartMaxBounds = {0.5f, 0.5f, 0.0f};
};

class Wall : public Basic_Object {
public:
    Wall();
    ~Wall() override; // Переопределение виртуального деструктора

    void init() override;   // Переопределение метода инициализации
    void draw() const override; // Переопределение метода отрисовки
    // destroy() будет вызвана из деструктора Basic_Object,
    // и его реализация в Basic_Object достаточна
    glm::vec3 get_bounds(int index);
    void move_bounds(float coords[3]);
    void rotate_bounds(float coords[3]);
private:
    glm::vec3 objectStartMinBounds = {-0.5, 0.0f, -0.5f};
    glm::vec3 objectStartMaxBounds = {0.5, 0.0f, 0.5f};
};

class Enemy : Basic_Object
{
public:
    Enemy();
    ~Enemy(); 
    void init() override;  
    void draw() const override; 
    glm::vec3 get_bounds(int index);
    void move_bounds(float coords[3]);
    void rotate_bounds(float coords[3]);
    bool isCameraLookingAt(const glm::vec3& cameraPos, const glm::vec3& cameraFront);
private:
    glm::vec3 objectStartMinBounds = {-0.5, 0.0f, -0.5f};
    glm::vec3 objectStartMaxBounds = {0.5, 0.0f, 0.5f};
};


#endif // BASIC_OBJECTS_H

