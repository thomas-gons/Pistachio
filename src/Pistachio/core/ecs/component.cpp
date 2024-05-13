//
// Created by thomas on 08/10/23.
//

#include "component.h"

#include <utility>


GraphicsComponent::GraphicsComponent(std::string tag, Sprite *sprite, Animation *an) :
        mkTag(std::move(tag)),
        mSprite(sprite) {

    if (an != nullptr) {

        uint8_t maxFrameCount = 0;
        for (uint8_t i = 0; i < an->mRowsCount; i++)
            maxFrameCount = std::max(maxFrameCount, an->mFrameCountPerRow[i]);

        an->mFrameW = sprite->getWidth() / maxFrameCount;
        an->mFrameH = sprite->getHeight() / an->mRowsCount;
        mAc = new AnimationComponent(*an);
    }

}
