//
// Created by thomas on 08/10/23.
//
#include "system.h"

#include <memory>


//void RenderSystem::update() {
//    for (size_t i = 0; i < _gc->size(); i++) {
//        auto gc = &_gc->at(i);
//        auto tc = &_tc->at(i);
//        auto ac = gc->ac;
//        if (ac && SDL_GetTicks() - ac->lastFrameTime > ac->animation.frameDuration) {
//            gc->srcRect->x = (gc->srcRect->x + ac->animation.sWidth) % ac->animation.aWidth;
//            gc->srcRect->y = ac->animation.currentRow * ac->animation.sHeight;
//            // TODO: handle rows
//            gc->srcRect->w = ac->animation.sWidth;
//            gc->srcRect->h = ac->animation.sHeight;
//            ac->currentFrame++;
//            ac->lastFrameTime = SDL_GetTicks();
//        }
//
//        auto destRect = new SDL_Rect({
//            static_cast<int>(tc->x),
//            static_cast<int>(tc->y),
//            gc->srcRect->w,
//            gc->srcRect->h
//        });
//
//        SDL_RenderCopy(
//                renderer, gc->sprite->texture,
//                gc->srcRect, destRect
//        );
//    }
//}

RenderSystem::RenderSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc) {
    _gc = &gc;
    _tc = &tc;
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);

    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    _vertices = std::unique_ptr<float[]>(vertices);

    _model = glm::mat4(1.0f);
    _projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(GLfloat) * 4 * 4,
            nullptr,
            GL_DYNAMIC_DRAW
    );

    glBindVertexArray(_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4  , GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindVertexArray(0);
}


void RenderSystem::update() {
    for (size_t i = 0; i < _gc->size(); i++) {
        auto gc = &_gc->at(i);
        auto tc = &_tc->at(i);
        auto ac = gc->ac;
        if (ac && SDL_GetTicks() - ac->lastFrameTime > ac->animation.frameDuration) {
            gc->srcRect->x = (gc->srcRect->x + ac->animation.sWidth) % ac->animation.aWidth;
            gc->srcRect->y = ac->animation.currentRow * ac->animation.sHeight;
        }
    }
}

// TODO: use key input callback define in glfw (see glfwSetKeyCallback)
int InputSystem::update(std::queue<SDL_Event> eventStream) {
    auto & inputComponent = _ic->at(0);
    while (!eventStream.empty()) {
        SDL_Event event = eventStream.front();
        eventStream.pop();
        SDL_Keycode keyCode = event.key.keysym.sym;
        if (event.type == SDL_KEYDOWN) {
            inputComponent.keyStates[keyCode] = true;
        }
        if (event.type == SDL_KEYUP) {
            inputComponent.keyStates[keyCode] = false;
        }

        if (keyCode == SDLK_ESCAPE) {
            return SIG_ESCAPE;
        }
    }
    return 0;
}
//
//void MovementSystem::update() {
//    auto ic = _ic->at(0);
//    float dx = 0, dy = 0;
//    int i = 0;
//    for (auto & tc : *_tc) {
//        if (i != 0) {
//            break;
//        }
//        if (ic.keyStates[SDLK_z]) {
//            dy--;
//        }
//        if (ic.keyStates[SDLK_q]) {
//            dx--;
//        }
//        if (ic.keyStates[SDLK_s]) {
//            dy++;
//        }
//        if (ic.keyStates[SDLK_d]) {
//            dx++;
//        }
//
//        if (dx != 0 && dy != 0) {
//            dx /= 1.41421356237;
//            dy /= 1.41421356237;
//        }
//
//        tc.x += dx;
//        tc.y += dy;
//        i++;
//    }
//}
