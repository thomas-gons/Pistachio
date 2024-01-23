//
// Created by thomas on 08/10/23.
//
#pragma once

#include "component.h"
#include "core/system.h"
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class RenderSystem : public System {
    Pool<GraphicsComponent> *_gc;
    Pool<TransformComponent> *_tc;

    GLuint _VAO;
    GLuint _VBO;
    std::unique_ptr<float[]> _vertices;
    glm::mat4 _model;
    glm::mat4 _projection;

public:
    RenderSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc);

    void update() override;
};


class InputSystem : public System {
    Pool<InputComponent> *_ic;

public:

    explicit InputSystem(Pool<InputComponent> & ic) : _ic(&ic) {};

    int update(std::queue<SDL_Event> eventStream);
};


class MovementSystem : public System {
    Pool<TransformComponent> *_tc;
    Pool<InputComponent> *_ic;

public:
    MovementSystem(Pool<TransformComponent> &tc, Pool<InputComponent> &ic) :
        _tc(&tc), _ic(&ic) {};

    void update() override;
};