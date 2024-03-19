//
// Created by thomas on 3/2/24.
//

#include "spriteBatch.h"


SpriteBatch::SpriteBatch(uint32_t nSprites, GraphicsComponent &gc) {
    if (_maxNSprites < nSprites) {
        _maxNSprites = nSprites;
        _quadsTmp.reserve(nSprites);
    }

    quads.reserve(nSprites);

    if (gc.ac == nullptr) {
        tex_wh = glm::vec2(1.0f, 1.0f);
    } else {
        tex_wh = glm::vec2(1 / gc.ac->animation.sWidth, 1 / gc.ac->animation.sHeight);
    }

    _bindingIndex = _globBindingIndex++;
    glGenBuffers(1, &_ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, SSBO_SIZE(nSprites), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ssbo);
}

void SpriteBatch::updateSSBO() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec2), &tex_wh);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec2), quads.size() * sizeof(QuadInfoNDC), quads.data());
}
