//
// Created by thomas on 22/01/24.
//
#include <iostream>
#include "texture.h"


bool Texture::loadTexture(const char* filename)
{
    unsigned char *data = stbi_load(filename, &this->width, &this->height, nullptr, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load texture: " << filename << std::endl;
        return false;
    }

    // Create a OpenGL texture identifier
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    // Setup standard filtering and wrapping options
    this->setTextureWraps(GL_REPEAT, GL_REPEAT);
    this->setTextureFilters(GL_LINEAR, GL_LINEAR);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    return true;
}

void Texture::setTextureWraps(GLint wrapS, GLint wrapT) const
{
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture::setTextureFilters(GLint minFilter, GLint magFilter) const
{
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}
