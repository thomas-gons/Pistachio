//
// Created by thomas on 08/10/23.
//

#include "component.h"

#include <utility>


GraphicsComponent::GraphicsComponent(std::string tag, Sprite *sprite, Animation *an) :
    tag(std::move(tag)),
    sprite(sprite) {

    renderSize = glm::vec2(sprite->getWidth(), sprite->getHeight());
    if (an != nullptr) {

        uint8_t maxFrameCount = *std::max(an->frameCountPerRow.begin(), an->frameCountPerRow.end());

        an->sWidth = sprite->getWidth() / maxFrameCount;
        an->sHeight = sprite->getHeight() / an->rowsCount;
        new AnimationComponent(*an);
    }

}

GraphicsComponent::GraphicsComponent(std::string tag, glm::vec2 renderSize, Sprite *sprite, Animation *an) :
    tag(std::move(tag)),
    renderSize(renderSize),
    sprite(sprite) {

    ac = (an == nullptr) ? nullptr : new AnimationComponent(*an);
}



TransformComponent::TransformComponent(double x, double y) : x(x), y(y) {}