//
// Created by thomas on 05/09/23.
//

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common.h"
#include "component.h"
#include "pool.h"
#include "spriteBatch.h"


class System {
public:
    virtual void update() {};
};

// Concrete systems

typedef struct {
    std::vector<uint32_t> componentsIndexes;
    SpriteBatch spriteBatch;
} SpriteBatchInfo;

class RenderSystem : public System {
    Pool<GraphicsComponent> &_gc;
    Pool<TransformComponent> &_tc;
    std::unordered_map<std::string, SpriteBatchInfo> _spriteBatches;

public:
    RenderSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc);
    void update(ResourceManager*);
};


class InputSystem : public System {
public:
    static std::array<bool, 348> _keys;
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};


class MovementSystem : public System {
    Pool<TransformComponent> *_tc{};

public:
    MovementSystem() = default;
    explicit MovementSystem(Pool<TransformComponent> &tc);

    void update() override;
};