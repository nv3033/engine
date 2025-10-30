#pragma once
#include <glad/glad.h>

#include "stb_image.h"

#include <iostream>

class Textures
{
private:
    const char* pathes[3] = {"/home/anton/Desktop/portfolio/engine/img/tex.png", 
        "/home/anton/Desktop/portfolio/engine/img/stone_tex.png", 
        "/home/anton/Desktop/portfolio/engine/img/enemy.png"};
    GLuint textures[3];
public:
    Textures(/* args */);
    ~Textures();
    void init();
    void activate(int index);
    void set_path(const char* path_to_set);
    GLuint* get_textures();
};

