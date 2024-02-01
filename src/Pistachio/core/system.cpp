//
// Created by thomas on 08/10/23.
//
#include "system.h"

extern ResourceManager *resourceManager;

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
    glGenBuffers(1, &_EBO);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &_VAO);
    glGenBuffers(1, &_VBO);
    glGenBuffers(1, &_EBO);

    glBindVertexArray(_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



}


void RenderSystem::update(ResourceManager *resourceManager) {
    Shader &shader = resourceManager->getShader("sprite");
    shader.use();
    for (size_t i = 0; i < _gc->size(); i++) {
        GraphicsComponent &gc = _gc->at(i);
        TransformComponent &tc = _tc->at(i);
        tc.x += 0.1;
        tc.y += 0.1;
        shader.setUniform("x_dir", tc.x);
        shader.setUniform("y_dir", tc.y);
        glBindTexture(GL_TEXTURE_2D, gc.sprite->getTextureID());
        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

//// TODO: use key input callback define in glfw (see glfwSetKeyCallback)
//int InputSystem::update(std::queue<SDL_Event> eventStream) {
//    auto & inputComponent = _ic->at(0);
//    while (!eventStream.empty()) {
//        SDL_Event event = eventStream.front();
//        eventStream.pop();
//        SDL_Keycode keyCode = event.key.keysym.sym;
//        if (event.type == SDL_KEYDOWN) {
//            inputComponent.keyStates[keyCode] = true;
//        }
//        if (event.type == SDL_KEYUP) {
//            inputComponent.keyStates[keyCode] = false;
//        }
//
//        if (keyCode == SDLK_ESCAPE) {
//            return 1;
//        }
//    }
//    return 0;
//}
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
