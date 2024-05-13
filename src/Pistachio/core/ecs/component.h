//
// Created by thomas on 02/09/23.
//

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <functional>

#include "resources/resourceManager.h"
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
    const std::string mkTag;
    Sprite *mSprite;

    struct AnimationComponent {
        Animation mAnimation;
        uint16_t mCurrentFrame = 0;
        uint8_t mCurrentRow{AnimationRow::WALK};
        double mLastFrameTime = 0;

        explicit AnimationComponent(Animation animation) : mAnimation(std::move(animation)) {}

        friend std::ostream &operator<<(std::ostream &os, const AnimationComponent &p_ac) {
            os << "AnimationComponent(" << p_ac.mAnimation
               << "currentFrame: " << p_ac.mCurrentFrame
               << ", currentRow: " << p_ac.mCurrentRow
               << ")";

            return os;
        }
    };

    AnimationComponent *mAc;

    GraphicsComponent(std::string tag, Sprite *sprite, Animation *an = nullptr);

    friend std::ostream& operator<<(std::ostream& os, const GraphicsComponent& gc) {
        os << "GraphicsComponent(tag: " << gc.mkTag
              << ", sprite: " << gc.mSprite
              << ", AnimationComponent: " << gc.mAc
              << ")";

        return os;
    }
};


struct TransformComponent : public Component {
    glm::dvec2 mPosition;
    float mRotation;
    float mScale;

    TransformComponent(double x, double y) : mPosition(x, y), mRotation(0), mScale(1) {}
    TransformComponent(double x, double y, float rotation, float scale) : mPosition(x, y), mRotation(rotation), mScale(scale) {}

    friend std::ostream& operator<<(std::ostream& os, const TransformComponent& tc) {
        os << "TransformComponent(position: " << tc.mPosition.x << ", " << tc.mPosition.y
              << ", rotation: " << tc.mRotation
              << ", scale: " << tc.mScale
              << ")";
        return os;
    }
};


static std::unordered_map<std::uint64_t, std::string> componentNames = {
    {typeid(GraphicsComponent).hash_code(), "GraphicsComponent"},
    {typeid(TransformComponent).hash_code(), "TransformComponent"},
};

#endif //__COMPONENT_H__