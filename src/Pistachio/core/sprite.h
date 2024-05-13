//
// Created by thomas on 04/09/23.
//

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "core/common.h"
#include "core/resources/texture.h"


/**
 * @brief A sprite is a generic texture that can be used to draw stuff on the screen.
 */
typedef Texture Sprite;

/**
 * @brief Simple enum to describe the animation cycle.
 */
enum AnimationCycle {
    LOOP,
    ONCE
};

/**
 * @brief Describe the type of each animation row.
 */
enum AnimationRow {
    IDLE,
    WALK,
    ATTACK,
    DEATH
};

/**
 * @brief When we deal with animated sprites, we need to know a lot of additional stuff.\n
 * This class lets us describe a set of animations in a sprite sheet, where each line represents a different animation.
 * @note Each frame has the same dimensions
 *
 * @see Sprite
 */
struct Animation {

    /// The duration of each frame in milliseconds (same for all animations)
    uint16_t mFrameDuration;

    /// The number of frames in each animation row
    std::vector<uint8_t> mFrameCountPerRow;

    /// The number of animations in the sprite sheet
    uint8_t mRowsCount;

    uint32_t mFrameW;
    uint32_t mFrameH;

    /// The texture coordinates of the sprite
    float mTexU;
    float mTexV;

    /**
     * @brief Construct an animation that will be referenced by a @c GraphicsComponent
     */
    Animation(uint16_t frameDuration, std::vector<uint8_t> frameCountPerRow, uint8_t rowsCount) :
            mFrameDuration(frameDuration / 60),
            mFrameCountPerRow(std::move(frameCountPerRow)),
            mRowsCount(rowsCount) {

        uint8_t maxFrames = 0;
        for (auto &frameCount : mFrameCountPerRow) {
            if (frameCount > maxFrames)
                maxFrames = frameCount;
        }
        mTexU = 1 / (float) maxFrames;
        mTexV = 1 / (float) rowsCount;
    }

    friend std::ostream &operator<<(std::ostream &os, const Animation &an) {
        os << "Animation(mFrameDuration: " << an.mFrameDuration;
        os << ", mFrameCountPerRow: [";
        for (auto &frameCount : an.mFrameCountPerRow) {
            os << ", frameCount: " << frameCount;
        }
        os << ", mRowsCount: " << an.mRowsCount
           << ", mWidth: " << an.mFrameW
              << ", mHeight: " << an.mFrameH
           << ")";

        return os;
    }

};

#endif //__SPRITE_H__