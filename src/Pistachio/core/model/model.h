//
// Created by thomas on 17/11/23.
//
#ifndef __MODEL_H__
#define __MODEL_H__

#include <fstream>
#include <utility>

#include "common.h"
#include "core/ecs/component.h"

extern ResourceManager resourceManager;

class Model {

private:
    std::string _mTag;
    std::vector<std::string> _mComponentTags;
    std::unordered_map<std::string, TComponentConstructor> _mComponentConstructors;

    void selectComponent(const std::string& componentTag, nlohmann::json args) {
        if (componentTag == "TransformComponent") {
            _mComponentTags.emplace_back("Transform");
            _mComponentConstructors["Transform"] = defineConstructor<TransformComponent>(
                    args["x"], args["y"]
            );
        } else if (componentTag == "GraphicsComponent") {
            _mComponentTags.emplace_back("Graphics");
            // TODO: handle sprite and animation model
            Sprite *sprite = nullptr;
            Animation *animation = nullptr;
            if (args.contains("AnimationComponent")) {
                animation = createFromJson<Animation>(
                        args["AnimationComponent"], "frameDuration", "frameCountPerRow"
                );
            }
            _mComponentConstructors["Graphics"] = defineConstructor<GraphicsComponent>(
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

    Model(const std::string& modelPath, std::string modelTag) : _mTag(std::move(modelTag)) {
        std::ifstream file(modelPath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open model file: " + modelPath);
        }
        nlohmann::json json;
        file >> json;
        for (auto& [componentName, args] : json.items())
            selectComponent(componentName, args);
    }

    std::vector<std::string> getComponentTags() {
        return _mComponentTags;
    }

    std::unordered_map<std::string, TComponentConstructor> getComponentConstructors() {
        return _mComponentConstructors;
    }

};

#endif //__MODEL_H__