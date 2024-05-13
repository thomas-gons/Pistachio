//
// Created by thomas on 08/10/23.
//
#include "system.h"
#include <ctime>

std::array<bool, 348> InputSystem::_msKeys;


/************** RENDER SYSTEM **************/
RenderSystem::RenderSystem(Pool<GraphicsComponent> &gcPool, Pool<TransformComponent> &tcPool) :
        _mGcPool(gcPool), _mTcPool(tcPool) {}


void RenderSystem::update() {
    // sprite batching
    for (auto &[tag, spriteBatchInfo] : _mSpriteBatchesInfo) {
        auto &spriteBatch = spriteBatchInfo->spriteBatch;
        double currentTime = glfwGetTime();
        Animation an = _mGcPool.at(spriteBatchInfo->mComponentsIndexes[0]).mAc->mAnimation;
        glBindTexture(GL_TEXTURE_2D, spriteBatch.mTextureID);
        std::uint32_t quadIndex = 0;
        for (uint32_t i : spriteBatchInfo->mComponentsIndexes) {
            auto gc = &_mGcPool.at(i);
            auto tc = &_mTcPool.at(i);
            auto ac = gc->mAc;

            if (an.mFrameDuration < currentTime - ac->mLastFrameTime) {
                ac->mCurrentFrame = (ac->mCurrentFrame + 1) % an.mFrameCountPerRow[ac->mCurrentRow];
                ac->mLastFrameTime = currentTime;
            }
            QuadInfoNDC *quadInfoNDC = &spriteBatch.mQuads[quadIndex];
            quadInfoNDC->mQuadXY = glm::vec2(
                    2 * (tc->mPosition.x / WIDTH) - 1, 2 * (tc->mPosition.y / HEIGHT) - 1
            );

            quadInfoNDC->mQuadWH = glm::vec2(
                    2 * (an.mFrameW * tc->mScale / WIDTH), 2 * (an.mFrameH * tc->mScale / HEIGHT)
            );

            quadInfoNDC->mTexUV = glm::vec2(
                    an.mTexU * (float) ac->mCurrentFrame,
                    an.mTexV * (float) ac->mCurrentRow
            );
//            std::cout << "Quad info: " << quadInfoNDC->mQuadXY.x << ", " << quadInfoNDC->mQuadXY.y << std::endl;
//            std::cout << "           " << quadInfoNDC->mQuadWH.x << ", " << quadInfoNDC->mQuadWH.y << std::endl;
//            std::cout << "           " << quadInfoNDC->mTexUV.x << ", " << quadInfoNDC->mTexUV.y << std::endl;
//            std::cout << "           " << spriteBatch.mTexWH.x << ", " << spriteBatch.mTexWH.y << std::endl;
//            std::cout << std::endl;
            quadIndex++;
        }
        // display the GPU usage
        spriteBatch.updateSSBO();
        spriteBatch.draw();
    }

}

void RenderSystem::addSpriteBatch(const std::string &tag) {
    std::vector <uint32_t> componentsIndexes;
    for (int i = 0; i < _mGcPool.size(); i++) {
        if (_mGcPool.at(i).mkTag == tag) {
            componentsIndexes.push_back(i);
        }
    }
    auto sb = SpriteBatch(componentsIndexes.size(), _mGcPool.at(componentsIndexes[0]));
    _mSpriteBatchesInfo[tag] = new SpriteBatchInfo{componentsIndexes, sb};
}

//// TODO: use key input callback define in glfw (see glfwSetKeyCallback)
void InputSystem::keyCallback(GLFWwindow *window, int keyCode, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        _msKeys[keyCode] = true;
    }
    if (action == GLFW_RELEASE) {
        _msKeys[keyCode] = false;
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
    for (auto &tc : _mTcPool) {
        if (i != 0) {
            break;
        }
        if (InputSystem::_msKeys[GLFW_KEY_W]) {
            dy += verticalDelta;
        }
        if (InputSystem::_msKeys[GLFW_KEY_A]) {
            dx -= 0.01;
        }
        if (InputSystem::_msKeys[GLFW_KEY_S]) {
            dy -= verticalDelta;
        }
        if (InputSystem::_msKeys[GLFW_KEY_D]) {
            dx += 0.01;
        }

        if (dx != 0 && dy != 0) {
            dx /= 1.41421356237;
            dy /= 1.41421356237;
        }

        tc.mPosition.x += dx;
        tc.mPosition.y += dy;
        i++;
    }
}

