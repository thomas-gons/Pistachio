//
// Created by thomas on 22/01/24.
//
#include "resourceManager.h"


void ResourceManager::loadTexture(const char *name, const char *path) {
    Texture texture = {};

    if (texture.loadTexture(path)) {
        mShaderTags.emplace_back(name);
        _mTextures[name] = texture;
    }
}

void ResourceManager::loadShader(const char *name, const char *path) {
    Shader shader = {};
    shader.processShader(path);
    mShaderTags.emplace_back(name);
    mShaders[name] = shader;
}

template<typename... Args>
void ResourceManager::loadAllTextures(Args... args) {
    (loadTexture(args), ...);
}

Texture& ResourceManager::getTexture(const char *name) {
    return _mTextures[name];
}

Shader& ResourceManager::getShader(const char *name) {
    return mShaders[name];
}

std::vector<std::string> ResourceManager::getTextureTags() {
    return mTextureTags;
}

std::vector<std::string> ResourceManager::getShaderTags() {
    return mShaderTags;
}