//
// Created by thomas on 17/11/23.
//

#pragma once

#include <fstream>
#include <utility>

#include "common.h"
#include "core/ecs/component.h"

extern ResourceManager resourceManager;

class Model {
    std::string tag;
    std::unordered_map<std::string, TComponentConstructor> componentConstructors;

    void selectComponent(const std::string& componentTag, nlohmann::json args) {
        if (componentTag == "TransformComponent") {
            componentConstructors["Transform"] = defineConstructor<TransformComponent>(
                    args["x"], args["y"]
            );
        } else if (componentTag == "GraphicsComponent") {
            // TODO: handle sprite and animation model
            Sprite *sprite = nullptr;
            Animation *animation = nullptr;
            if (args.contains("AnimationComponent")) {
                animation = createFromJson<Animation>(
                        args["AnimationComponent"], "frameDuration", "rowsCount", "frameCountPerRow"
                );
            }
             componentConstructors["Graphics"] = defineConstructor<GraphicsComponent>(
                     componentTag, sprite, animation
             );
        } else {
            throw std::runtime_error("Invalid component tag: " + componentTag);
        }
    }

    template <typename Tag, typename... Args>
    TComponentConstructor defineConstructor(const Args&... args) {
        static_assert(std::is_constructible_v<Tag, Args...>,
                      "Invalid constructor arguments for the specified type tag.");

        return [args...]() {
            return std::make_shared<Tag>(args...);
        };
    }

public:

    Model(const std::string& modelPath, std::string modelTag ) : tag(std::move(modelTag)) {
        std::ifstream file(modelPath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open model file: " + modelPath);
        }
        nlohmann::json json;
        file >> json;
        for (auto& [componentName, args] : json.items())
            selectComponent(componentName, args);
    }

    Model() = default;

    std::unordered_map<std::string, TComponentConstructor> getComponentConstructors() {
        return componentConstructors;
    }
};