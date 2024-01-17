//
// Created by thomas on 17/01/24.
//

//
// Created by thomas on 17/01/24.
//

#pragma once

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>


class Shader {
private:

    std::vector<GLint> shadersIDs;
    GLuint shaderProgram;

    template <typename T>
    void setUniformValue(GLint location, T value) const;

    template <typename T>
    void getUniformValue(GLint location, T& value) const;

public:
    Shader() = default;

    void addShader(const char *path, GLenum shaderType) {
        std::ifstream shaderFile;
        std::string pathStr = "resources/shaders/";
        pathStr += path;
        shaderFile.open(pathStr);
        if (!shaderFile.is_open()) {
            std::cout << "Failed to open shader file: " << pathStr << std::endl;
            return;
        }

        std::string shaderSource((std::istreambuf_iterator<char>(shaderFile)),
                                 std::istreambuf_iterator<char>());

        shaderFile.close();

        GLint shaderId = glCreateShader(shaderType);
        const char *shaderSourceCStr = shaderSource.c_str();
        glShaderSource(shaderId, 1, &shaderSourceCStr, nullptr);
        shadersIDs.push_back(shaderId);
    }

    void compile() {
        shaderProgram = glCreateProgram();
        for (auto &shaderId : shadersIDs) {
            glCompileShader(shaderId);
            GLint success;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
                std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
                return;
            }

            glAttachShader(shaderProgram, shaderId);
        }
        glLinkProgram(shaderProgram);

        // check for linking errors
        GLint success;
        char infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    GLuint getShaderProgram() {
        return shaderProgram;
    }

    void deleteAllShaders() {
        for (auto &shaderId : shadersIDs) {
            glDeleteShader(shaderId);
        }
    }

public:
    template <typename T>
    void setUniform(const char *name, T value) const {
        GLint location = glGetUniformLocation(shaderProgram, name);
        if (location != -1) {
            setUniformValue<T>(location, value);
        } else {
            std::cout << "Uniform '" << name << "' not found in the shader. (setter)" << std::endl;
            exit(-1);
        }
    }

    template <typename T>
    void getUniform(const char *name, T& value) const {
        GLint location = glGetUniformLocation(shaderProgram, name);
        if (location != -1) {
            getUniformValue<T>(location, value);
        } else {
            std::cout << "Uniform '" << name << "' not found in the shader. (getter)" << std::endl;
            exit(-1);
        }
    }
};

    template <>
    void Shader::setUniformValue<float>(GLint location, float value) const {
        glUniform1f(location, value);
    }

    template <>
    void Shader::setUniformValue<int>(GLint location, int value) const {
        glUniform1i(location, value);
    }

    template <>
    void Shader::setUniformValue<bool>(GLint location, bool value) const {
        glUniform1i(location, value);
    }

    template <>
    void Shader::setUniformValue<glm::vec2>(GLint location, glm::vec2 value) const {
        glUniform2f(location, value.x, value.y);
    }

    template <>
    void Shader::setUniformValue<glm::vec3>(GLint location, glm::vec3 value) const {
        glUniform3f(location, value.x, value.y, value.z);
    }

    template <>
    void Shader::setUniformValue<glm::vec4>(GLint location, glm::vec4 value) const {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    // ...

    template <>
    void Shader::getUniformValue<float>(GLint location, float& value) const {
        glGetUniformfv(shaderProgram, location, &value);
    }

    template <>
    void Shader::getUniformValue<int>(GLint location, int& value) const {
        glGetUniformiv(shaderProgram, location, &value);
    }

    template <>
    void Shader::getUniformValue<bool>(GLint location, bool& value) const {
        glGetUniformiv(shaderProgram, location, reinterpret_cast<GLint *>(&value));
    }

    template <>
    void Shader::getUniformValue<glm::vec2>(GLint location, glm::vec2& value) const {
        glGetUniformfv(shaderProgram, location, &value[0]);
    }
