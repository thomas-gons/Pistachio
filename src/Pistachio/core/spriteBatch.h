//
// Created by thomas on 3/2/24.
//

#ifndef __SPRITE_BATCH_H__
#define __SPRITE_BATCH_H__


#include <GL/glew.h>
#include <glm/glm.hpp>

#include "core/common.h"
#include "core/ecs/component.h"

#define SSBO_SIZE(n) (n * sizeof(QuadInfoNDC) + sizeof(glm::vec2))
#define VERTEX_SIZE 6

typedef struct QuadInfoNDC {
    glm::fvec2 mQuadXY;
    glm::fvec2 mQuadWH;
    glm::fvec2 mTexUV;
} QuadInfoNDC;


class SpriteBatch {

private:
    const static uint32_t _mkMaxNSprites = std::numeric_limits<uint32_t>::max();
    static GLuint _msGlobBindingIndex;

    GLuint _mVao = 0;
    GLuint _mSsbo = 0;
    GLuint _mBindingIndex;

public:
    std::vector<QuadInfoNDC> mQuads;
    glm::fvec2 mTexWH;
    GLuint mTextureID;


    SpriteBatch(uint32_t nSprites, GraphicsComponent &gc);
    void updateSSBO();
    void draw() const;
};


#endif // __SPRITE_BATCH_H__