//
// Created by thomas on 08/10/23.
//
#include "system.h"

std::array<bool, 348> InputSystem::_keys;


/************** RENDER SYSTEM **************/
RenderSystem::RenderSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc) :
    _gc(gc), _tc(tc) {}


void RenderSystem::update() {
    // sprite batching
    for (auto &[tag, spriteBatchInfo] : _spriteBatchesInfo) {
        auto &spriteBatch = spriteBatchInfo.spriteBatch;
        double currentTime = glfwGetTime();
        for (uint32_t i : spriteBatchInfo.componentsIndexes) {
            auto gc = &_gc.at(i);
            auto tc = &_tc.at(i);
            auto ac = gc->ac;

            if (ac->animation.frameDuration < currentTime - ac->lastFrameTime) {
                ac->currentFrame = (ac->currentFrame + 1) % ac->animation.frameCountPerRow[ac->currentRow];
                ac->lastFrameTime = currentTime;
            }

            QuadInfoNDC &quadInfoNDC = spriteBatch.quads[i];
            quadInfoNDC.quad_xy = glm::vec2(
                    2 * tc->x / 1980, tc->y / 1080
            );

            quadInfoNDC.quad_wh = glm::vec2(
                    2 * gc->renderSize.x / 1980, gc->renderSize.y / 1080
            );

            quadInfoNDC.tex_uv = glm::vec2(
                    ac->animation.tex_u * (float) ac->currentFrame,
                    ac->animation.tex_v * (float) ac->currentRow
            );
        }
        spriteBatch.updateSSBO();
        spriteBatch.draw();
    }

}

void RenderSystem::addSpriteBatch(const std::string &tag) {
    _spriteBatchesInfo[tag] = SpriteBatchInfo{
            std::vector<uint32_t>()
    };
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
