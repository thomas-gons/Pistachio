//
// Created by thomas on 17/01/24.
//

#ifndef __SHADER_H__
#define __SHADER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {

private:
    std::unordered_map<GLenum, std::string> loadShader(const char *path);
    void compileShader(std::unordered_map<GLenum, std::string> &shaderSources);

    template <typename T>
    void setUniformValue(GLint location, T value) const {};

    template <typename T>
    void getUniformValue(GLint location, T& value) const {};

public:
    GLuint mShaderProgram;

    void processShader(const char *path);

    void use() const;

    template <typename T>
    void setUniform(const char *name, T value) const {
        GLint location = glGetUniformLocation(mShaderProgram, name);
        if (location != -1) {
            setUniformValue<T>(location, value);
        } else {
            std::cout << "Uniform '" << name << "' not found in the shader. (setter)" << std::endl;
        }
    }

    template <typename T>
    void getUniform(const char *name, T& value) const {
        GLint location = glGetUniformLocation(mShaderProgram, name);
        if (location != -1) {
            if constexpr (std::is_pointer<T>::value) {
                getUniformValue<T>(location, glm::value_ptr(&value));
            } else {
                getUniformValue<T>(location, &value);
            }
        } else {
            std::cout << "Uniform '" << name << "' not found in the shader. (getter)" << std::endl;
            exit(-1);
        }
    }
};

#endif //__SHADER_H__