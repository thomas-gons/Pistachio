//
// Created by thomas on 3/2/24.
//

#ifndef SPRITEBATCH_H
#define SPRITEBATCH_H


#include <GL/glew.h>
#include <glm/glm.hpp>

#include "core/common.h"
#include "core/ecs/component.h"

#define SSBO_SIZE(n) (n * sizeof(QuadInfoNDC) + sizeof(glm::vec2))
#define VERTEX_SIZE 6

typedef struct QuadInfoNDC {
    glm::vec2 quad_xy;
    glm::vec2 quad_wh;
    glm::vec2 tex_uv;
} QuadInfoNDC;


class SpriteBatch {

private:
    const static uint32_t _maxNSprites = std::numeric_limits<uint32_t>::max();
    static GLuint _globBindingIndex;

    GLuint _ssbo = 0;
    GLuint _bindingIndex;

public:
    std::vector<QuadInfoNDC> quads;
    glm::vec2 tex_wh;

    explicit SpriteBatch(uint32_t nSprites, GraphicsComponent &gc);
    void updateSSBO();
    void draw() const;
};


#endif //SPRITEBATCH_H