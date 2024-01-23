//
// Created by thomas on 22/01/24.
//
#include "resourceManager.h"


void ResourceManager::loadTexture(const char *name, const char *path) {
    Texture texture = {};
    if (texture.loadTexture(path)) {
        this->textures[name] = texture;
    }
}

void ResourceManager::loadShader(const char *name, const char *path) {
    Shader shader = {};
    shader.processShader(path);
    this->shaders[name] = shader;
}

template<typename... Args>
void ResourceManager::loadAllTextures(Args... args) {
    (this->loadTexture(args), ...);
}

Texture& ResourceManager::getTexture(const char *name) {
    return this->textures[name];
}

Shader& ResourceManager::getShader(const char *name) {
    return this->shaders[name];
}