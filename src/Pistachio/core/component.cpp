//
// Created by thomas on 08/10/23.
//

#include "component.h"

#include <utility>


// This file only contains the implementation of the constructors of the components.
GraphicsComponent::GraphicsComponent(std::string tag, Sprite *sprite, Animation *an) :
    tag(std::move(tag)),
    sprite(sprite) {

    if (an) {
        uint8_t maxFrameCount;
        for (auto &i : an->frameCountPerRow) {
            if (i > maxFrameCount) {
                maxFrameCount = i;
            }
        }

        an->sWidth = sprite->getWidth() / maxFrameCount;
        an->sHeight = sprite->getHeight() / an->rowsCount;
//        an->aWidth = an->sWidth * an->frameCountPerRow[an->currentRow];
        ac = new AnimationComponent(*an);
    }
}

TransformComponent::TransformComponent(double x, double y) : x(x), y(y) {}