//
// Created by thomas on 08/10/23.
//
#include "system.h"

std::array<bool, 348> InputSystem::_keys;

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
        shader.setUniform("x_dir", (float) tc.x);
        shader.setUniform("y_dir", (float) tc.y);
        glBindTexture(GL_TEXTURE_2D, gc.sprite->getTextureID());
        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
