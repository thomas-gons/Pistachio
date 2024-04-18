//
// Created by thomas on 3/2/24.
//

#include "spriteBatch.h"


SpriteBatch::SpriteBatch(uint32_t nSprites, GraphicsComponent &gc) {
    if (nSprites > _maxNSprites)
        nSprites = _maxNSprites;

    quads.reserve(nSprites);

    // no animation => full texture to be displayed
    tex_wh = (gc.ac == nullptr) ?
            glm::vec2(1.0f, 1.0f) :
            glm::vec2(1 / gc.ac->animation.sWidth, 1 / gc.ac->animation.sHeight);

    _bindingIndex = _globBindingIndex++;
    glGenBuffers(1, &_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);

    // Allocate space for the SSBO on the GPU
    glBufferData(GL_SHADER_STORAGE_BUFFER, SSBO_SIZE(nSprites), nullptr, GL_DYNAMIC_DRAW);

    // Bind the SSBO to its binding index
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, _bindingIndex, _ssbo);
}

/**
 * Update the SSBO with the current state of the quads
 * by copying texture width and height and the quads data to the GPU
 */
inline void SpriteBatch::updateSSBO() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec2), &tex_wh);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2), quads.size() * sizeof(QuadInfoNDC), quads.data());
}

inline void SpriteBatch::draw() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
    glDrawArrays(GL_POINTS, 0, VERTEX_SIZE * quads.size());

}
