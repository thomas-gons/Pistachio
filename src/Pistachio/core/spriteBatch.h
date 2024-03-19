//
// Created by thomas on 3/2/24.
//

#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "core/common.h"
#include "core/component.h"

#define SSBO_SIZE(n) (n * sizeof(QuadInfoNDC) + sizeof(glm::vec2))


typedef struct QuadInfoNDC {
    glm::vec2 quad_xy;
    glm::vec2 quad_wh;
    glm::vec2 tex_uv;
} QuadInfoNDC;


class SpriteBatch {

private:
    static std::vector<QuadInfoNDC> _quadsTmp;
    static uint32_t _maxNSprites;
    static GLuint _globBindingIndex;

    GLuint _ssbo;
    GLuint _bindingIndex;

public:
    std::vector<QuadInfoNDC> quads;
    glm::vec2 tex_wh;

    explicit SpriteBatch(uint32_t nSprites, GraphicsComponent &gc);
    void updateSSBO();
};

