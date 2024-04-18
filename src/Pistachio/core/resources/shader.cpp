//
// Created by thomas on 22/01/24.
//
#include "shader.h"

void Shader::processShader(const char *path) {
    std::unordered_map<GLenum, std::string> shaderSources = loadShader(path);
    compileShader(shaderSources);
}

std::unordered_map<GLenum, std::string> Shader::loadShader (const char *path) {
    std::ifstream stream(path);
    if (!stream.is_open()) {
        std::cout << "Failed to open shader file: " << path << std::endl;
        exit(-1);
    }

    std::string line;
    GLenum shaderType = GL_NONE;
    std::unordered_map<GLenum, std::ostringstream> shaderSources;
    while (std::getline(stream, line)) {
        if (line.compare(0, 7, "#shader") == 0) {
            std::string shaderTypeHeader = line.substr(8);
            if (shaderTypeHeader == "vertex") {
                shaderType = GL_VERTEX_SHADER;
            } else if (shaderTypeHeader == "fragment") {
                shaderType = GL_FRAGMENT_SHADER;
            } else if (shaderTypeHeader == "geometry") {
                shaderType = GL_GEOMETRY_SHADER;
            } else {
                std::cout << "Unknown shader type: " << shaderType << std::endl;
                exit(-1);
            }
        } else {
            if (shaderType == GL_NONE) {
                std::cout << "No shader type specified!" << std::endl;
                exit(-1);
            }
            shaderSources[shaderType] << line << '\n';
        }
    }
    std::unordered_map<GLenum, std::string> shaderSourcesString;
    for (const auto& [type, sourceCode] : shaderSources) {
        shaderSourcesString[type] = sourceCode.str();
    }
    return shaderSourcesString;
}

void Shader::compileShader(std::unordered_map<GLenum, std::string> &shaderSources) {
    shaderProgram = glCreateProgram();
    if (shaderProgram == 0) {
        std::cout << "Shader program creation failed!" << std::endl;
        exit(-1);
    }
    for (const auto& [type, sourceCode] : shaderSources) {
        GLuint shader = glCreateShader(type);
        const char *source = sourceCode.c_str();
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cout << "Shader compilation failed: " << infoLog << std::endl;
            exit(-1);
        }
        glAttachShader(shaderProgram, shader);
        glDeleteShader(shader);
    }

    glLinkProgram(shaderProgram);
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Shader linking failed: " << infoLog << std::endl;
        exit(-1);
    }
}

void Shader::use() const {
    glUseProgram(shaderProgram);
}

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

template <>
void Shader::setUniformValue<glm::mat4>(GLint location, glm::mat4 value) const {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

