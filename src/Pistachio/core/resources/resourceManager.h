//
// Created by thomas on 22/01/24.
//

#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include <unordered_map>
#include <string>
#include <vector>

#include "texture.h"
#include "shader.h"


class ResourceManager {

private:
    std::unordered_map<std::string, Texture> _mTextures;
    std::unordered_map<std::string, Shader> mShaders;

public:

    std::vector<std::string> mTextureTags;
    std::vector<std::string> mShaderTags;

    ResourceManager() = default;

    void loadTexture(const char *name, const char *path);
    void loadShader(const char *name, const char *path);

    template<typename... Args>
    void loadAllTextures(Args... args);


    Texture& getTexture(const char *name);
    Shader& getShader(const char *name);

    std::vector<std::string> getTextureTags();
    std::vector<std::string> getShaderTags();
};

#endif //__RESOURCE_MANAGER_H__