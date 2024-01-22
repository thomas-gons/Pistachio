//
// Created by thomas on 22/01/24.
//

#include "resourceManager.h"


void resourceManager::loadTexture(const char *name, const char *path) {
    Texture texture;
    if (texture.loadTexture(path)) {
        this->textures[name] = texture;
    }
}

void resourceManager::loadShader(const char *name, const char *path) {
    Shader shader;
    shader.processShader(path);
    this->shaders[name] = shader;
}

template<typename... Args>
void resourceManager::loadAllTextures(Args... args) {
    (this->loadTexture(args), ...);
}

Texture& resourceManager::getTexture(const char *name) {
    return this->textures[name];
}

Shader& resourceManager::getShader(const char *name) {
    return this->shaders[name];
}