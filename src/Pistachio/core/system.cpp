//
// Created by thomas on 08/10/23.
//
#include "system.h"

std::array<bool, 348> InputSystem::_keys;

extern ResourceManager *resourceManager;

RenderSystem::RenderSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc) :
    _gc(gc), _tc(tc){}


void RenderSystem::update(ResourceManager *resourceManager) {
    for (size_t i = 0; i < _gc->size(); i++) {
        auto gc = &_gc->at(i);
        auto tc = &_tc->at(i);
        auto ac = gc->ac;
        double ndcX = (tc->x / 1920) * 2 - 1;
        double ndcY = (tc->y / 1080) * 2 - 1;
    }
}

//// TODO: use key input callback define in glfw (see glfwSetKeyCallback)
void InputSystem::keyCallback(GLFWwindow *window, int keyCode, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        _keys[keyCode] = true;
    }
    if (action == GLFW_RELEASE) {
        _keys[keyCode] = false;
    }

    if (keyCode == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

const float verticalDelta = (16.f/9.f) * 0.01;

//
void MovementSystem::update() {
    float dx = 0, dy = 0;
    int i = 0;
    for (auto & tc : *_tc) {
        if (i != 0) {
            break;
        }
        if (InputSystem::_keys[GLFW_KEY_W]) {
            dy += verticalDelta;
        }
        if (InputSystem::_keys[GLFW_KEY_A]) {
            dx -= 0.01;
        }
        if (InputSystem::_keys[GLFW_KEY_S]) {
            dy -= verticalDelta;
        }
        if (InputSystem::_keys[GLFW_KEY_D]) {
            dx += 0.01;
        }

        if (dx != 0 && dy != 0) {
            dx /= 1.41421356237;
            dy /= 1.41421356237;
        }

        tc.x += dx;
        tc.y += dy;
        i++;
    }
}

MovementSystem::MovementSystem(Pool<TransformComponent> &tc) {
    _tc = &tc;
}
