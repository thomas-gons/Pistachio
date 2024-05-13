//
// Created by thomas on 3/2/24.
//

#include "spriteBatch.h"


GLuint SpriteBatch::_msGlobBindingIndex = 0;

SpriteBatch::SpriteBatch(uint32_t nSprites, GraphicsComponent &gc) {

    mTextureID = gc.mSprite->getTextureID();
    GraphicsComponent::AnimationComponent *ac = gc.mAc;

    if (nSprites > _mkMaxNSprites)
        nSprites = _mkMaxNSprites;

    mQuads = std::vector<QuadInfoNDC>(nSprites);
    // no animation => full texture to be displayed
    mTexWH = (ac == nullptr) ?
             glm::fvec2(1.0f, 1.0f) :
             glm::fvec2(ac->mAnimation.mTexU, ac->mAnimation.mTexV);

    glGenVertexArrays(1, &_mVao);

    _mBindingIndex = SpriteBatch::_msGlobBindingIndex++;
    glGenBuffers(1, &_mSsbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _mSsbo);

    // Allocate space for the SSBO on the GPU
    glBufferData(GL_SHADER_STORAGE_BUFFER, SSBO_SIZE(nSprites), nullptr, GL_DYNAMIC_DRAW);
    // Bind the SSBO to its binding index
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _mBindingIndex, _mSsbo);
}

/**
 * Update the SSBO with the current state of the quads
 * by copying texture width and height and the quads data to the GPU
 */
void SpriteBatch::updateSSBO() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _mSsbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::fvec2), &mTexWH);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::fvec2), mQuads.size() * sizeof(QuadInfoNDC), mQuads.data());
}

void SpriteBatch::draw() const {
    glBindVertexArray(_mVao);
    glDrawArrays(GL_POINTS, 0, VERTEX_SIZE * mQuads.size());
}
