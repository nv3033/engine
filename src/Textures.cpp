#include "Textures.h"

Textures::Textures(/* args */)
{
}

Textures::~Textures()
{
    glDeleteTextures(2, textures);
}

void Textures::init(){
    // 4. Загрузка текстуры
    glGenTextures(sizeof(textures), textures);
    for (int i = 0; i < sizeof(textures); i ++){
        glBindTexture(GL_TEXTURE_2D, textures[i]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        unsigned char* data = stbi_load(pathes[i], &width, &height, &nrChannels, 0); // Убедитесь, что tex.png существует
        if (data) {
            GLenum format;
            if (nrChannels == 1)      format = GL_RED;
            else if (nrChannels == 3) format = GL_RGB;
            else if (nrChannels == 4) format = GL_RGBA;
            else { std::cerr << "Unsupported image format" << std::endl; }

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture from img/tex.png" << std::endl;
        }
        stbi_image_free(data);
    }
}
void Textures::activate(int index){
    // Активация текстуры
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[index]);
}

GLuint* Textures::get_textures(){
    return textures;
}

//void Texture::set_path(const char* path_to_set){
//    path = path_to_set;
//}
