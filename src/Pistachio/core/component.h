//
// Created by thomas on 02/09/23.
//

#pragma once

#include <functional>

#include "resourceManager/resourceManager.h"
#include "sprite.h"


using TComponentUUID = std::uint32_t;
using ComponentHash = std::size_t;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;
};

using TComponentConstructor = std::function<std::shared_ptr<Component>()>;


// Concrete components

struct GraphicsComponent : public Component {
    const std::string& tag;
    Sprite *sprite;

    struct AnimationComponent {
        Animation animation;
        uint16_t currentFrame = 0;
        uint16_t lastFrameTime = 0;

        explicit AnimationComponent(Animation an) : animation(std::move(an)) {}
    };

    AnimationComponent *ac;
    GraphicsComponent(const std::string& tag, Sprite *sprite, Animation *an = nullptr);

    friend std::ostream& operator<<(std::ostream& os, const GraphicsComponent& gc) {
        os << "GraphicsComponent(tag: " << gc.tag << ")";
        return os;
    }
};


struct TransformComponent : public Component {
    double x;
    double y;
    TransformComponent(double x, double y);

    friend std::ostream& operator<<(std::ostream& os, const TransformComponent& tc) {
        os << "TransformComponent(x: " << tc.x << ", y: " << tc.y << ")";
        return os;
    }
};


struct InputComponent : public Component {
    std::unordered_map<GLint, bool> keyStates;
    explicit InputComponent(const std::vector<GLint>& keyCodes);
};