//
// Created by thomas on 22/01/24.
//

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <unordered_map>
#include <string>

#include "texture.h"
#include "shader.h"


class ResourceManager {

private:
    std::unordered_map<std::string, Texture> _mTextures;
    std::unordered_map<std::string, Shader> mShaders;

public:

    ResourceManager() = default;

    void loadTexture(const char *name, const char *path);
    void loadShader(const char *name, const char *path);

    template<typename... Args>
    void loadAllTextures(Args... args);


    Texture& getTexture(const char *name);
    Shader& getShader(const char *name);
};

#endif //__RESOURCE_MANAGER_H__