//
// Created by thomas on 08/10/23.
//

#include "component.h"


// This file only contains the implementation of the constructors of the components.
GraphicsComponent::GraphicsComponent(const std::string& tag, Sprite *sprite, Animation *an) :
    tag(tag),
    sprite(sprite) {

    srcRect = new SDL_Rect({0, 0, sprite->width, sprite->height});

    if (an) {
        uint8_t maxFrameCount;
        for (auto &i : an->frameCountPerRow) {
            if (i > maxFrameCount) {
                maxFrameCount = i;
            }
        }

        an->sWidth = sprite->width / maxFrameCount;
        an->sHeight = sprite->height / an->rowsCount;
        an->aWidth = an->sWidth * an->frameCountPerRow[an->currentRow];
        ac = new AnimationComponent(*an);
        srcRect->w = an->sWidth;
        srcRect->h = an->sHeight;
    }
}


TransformComponent::TransformComponent(double x, double y) : x(x), y(y) {}


InputComponent::InputComponent(const std::vector<GLFWkeyCode>& keyCodes) {
    for (auto &keyCode : keyCodes) {
        keyStates[keyCode] = false;
    }
}
