//
// Created by thomas on 08/10/23.
//
#pragma once

#include <SDL_keycode.h>

#include "core/component.h"
#include "sprite.h"


struct GraphicsComponent : public Component {
    const std::string& tag;
    Sprite *sprite;
    SDL_Rect *srcRect;

    struct AnimationComponent {
        Animation animation;
        uint16_t currentFrame = 0;
        uint16_t lastFrameTime = 0;

        explicit AnimationComponent(Animation an) : animation(std::move(an)) {}
    };

    AnimationComponent *ac;
    GraphicsComponent(const std::string& tag, Sprite *sprite, Animation *an = nullptr);
};


struct TransformComponent : public Component {
    double x;
    double y;
    TransformComponent(double x, double y);
};


struct InputComponent : public Component {
    std::unordered_map<SDL_Keycode, bool> keyStates;
    explicit InputComponent(const std::vector<SDL_Keycode>& keyCodes);
};

