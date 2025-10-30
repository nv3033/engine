#ifndef BASIC_OBJECTS_H
#define BASIC_OBJECTS_H

#include <glad/glad.h>
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
};

#endif // BASIC_OBJECTS_H

