//
// Created by thomas on 22/01/24.
//

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <iostream>
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

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    GLuint getTextureID() const { return textureID; }
};

#endif //__TEXTURE_H__