#pragma once
#include <glad/glad.h>

#include <iostream>
#include <string>

/*class Animal {
public:
    Animal(std::string name) : name_(name) {}
    virtual void makeSound() { std::cout << "Generic animal sound" << std::endl; } // Virtual for polymorphism
protected:
    std::string name_;
};

class Dog : public Animal {
public:
    Dog(std::string name, std::string breed) : Animal(name), breed_(breed) {}
    void makeSound() override { std::cout << "Woof!" << std::endl; }
    void wagTail() { std::cout << name_ << " is wagging its tail" << std::endl; }
private:
    std::string breed_;
};

int main() {
    Dog myDog("Fido", "Labrador");
    myDog.makeSound(); // Output: Woof!
    myDog.wagTail();   // Output: Fido is wagging its tail

    Animal* animalPtr = &myDog;  // Polymorphism
    animalPtr->makeSound();       // Output: Woof! (calls Dog's makeSound due to 'virtual')

    return 0;
}*/

class Basic_Object
{
protected:
    GLuint VAO, VBO;
    void destroy();
public:
    Basic_Object();

    virtual void init();
    void draw() const;
};

class Wall : public Basic_Object{
public:
    ~Wall();
    void init() override;
};

class Cube : public Basic_Object{
public:
    ~Cube();
    void init() override;
};
