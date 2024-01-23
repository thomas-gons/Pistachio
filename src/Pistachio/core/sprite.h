//
// Created by thomas on 04/09/23.
//

#include "core/common.h"
#include "core/resourceManager/texture.h"

#pragma once

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
    /// The number of frames in each animation row
    std::vector<uint8_t> frameCountPerRow;

    /// The duration of each frame in milliseconds (same for all animations)
    uint16_t frameDuration;

    /// The number of animations in the sprite sheet
    uint8_t rowsCount;

    AnimationRow currentRow{AnimationRow::WALK};

    /**
     * @brief The width of the current animation row
     * @note This is not the width of each frame
     */
    uint16_t aWidth{};

    /// The width of each frame
    uint16_t sWidth{};

    /// The height of each frame
    uint16_t sHeight{};

    /**
     * @brief Construct an animation that will be referenced by a @c GraphicsComponent
     */
    Animation(uint16_t frameDuration, uint8_t rowsCount, std::vector<uint8_t> frameCountPerRow) :
        frameDuration(frameDuration),
        rowsCount(rowsCount),
        frameCountPerRow(std::move(frameCountPerRow)) {}
};


