//
// Created by thomas on 05/09/23.
//

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common.h"
#include "core/ecs/component.h"
#include "core/ecs/pool.h"
#include "spriteBatch.h"


class System {
public:
    virtual void update() {};
};

// Concrete systems


/************** RENDER SYSTEM **************/
typedef struct {
    std::vector<uint32_t> mComponentsIndexes;
    SpriteBatch spriteBatch;
} SpriteBatchInfo;

class RenderSystem : public System {
    Pool<GraphicsComponent> &_mGcPool;
    Pool<TransformComponent> &_mTcPool;
    std::unordered_map<std::string, SpriteBatchInfo*> _mSpriteBatchesInfo;

public:
    RenderSystem(Pool<GraphicsComponent> &gcPool, Pool<TransformComponent> &tcPool);
    void addSpriteBatch(const std::string &tag);
    void update() override;
};


/************** INPUT SYSTEM **************/
class InputSystem : public System {
public:
    static std::array<bool, 348> _msKeys;
    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};


/************** MOVEMENT SYSTEM **************/
class MovementSystem : public System {
    Pool<TransformComponent> &_mTcPool;

public:
    explicit MovementSystem(Pool<TransformComponent> &tc) : _mTcPool(tc) {}

    void update() override;
};

#endif //__SYSTEM_H__