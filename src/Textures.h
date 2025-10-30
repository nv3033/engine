#pragma once
#include <glad/glad.h>

#include "stb_image.h"

#include <iostream>

class Textures
{
private:
    const char* pathes[2] = {"/home/anton/Desktop/portfolio/texture_test/img/tex.png", "/home/anton/Desktop/portfolio/texture_test/img/stone_tex.png"};
    GLuint textures[2];
public:
    Textures(/* args */);
    ~Textures();
    void init();
    void activate(int index);
    void set_path(const char* path_to_set);
    GLuint* get_textures();
};

