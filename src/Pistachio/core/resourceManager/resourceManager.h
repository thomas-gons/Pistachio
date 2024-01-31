//
// Created by thomas on 22/01/24.
//

#pragma once

#include <unordered_map>
#include <string>

#include "texture.h"
#include "shader.h"


class ResourceManager {
    std::unordered_map<std::string, Texture> textures;
    std::unordered_map<std::string, Shader> shaders;

public:

    void loadTexture(const char *name, const char *path);
    void loadShader(const char *name, const char *path);

    template<typename... Args>
    void loadAllTextures(Args... args);


    Texture& getTexture(const char *name);
    Shader& getShader(const char *name);
};
