//
// Created by thomas on 22/01/24.
//
#pragma once

#include <GL/glew.h>
#include <stb_image.h>

#define STB_IMAGE_IMPLEMENTATION


class Texture {
    GLuint textureID;
    int width, height;

public:
    bool loadTexture(const char* filename);

    void setTextureWraps(GLint wrapS, GLint wrapT) const;
    void setTextureFilters(GLint minFilter, GLint magFilter) const;
};