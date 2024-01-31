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


class System {
public:
    virtual void update() {};
};

// Concrete systems

class RenderSystem : public System {
    Pool<GraphicsComponent> *_gc;
    Pool<TransformComponent> *_tc;

    GLuint _VAO;
    GLuint _VBO;
    GLuint _EBO;
    glm::mat4 _model;
    glm::mat4 _projection;

public:
    RenderSystem() = default;
    RenderSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc);
    void update() override;
};


class InputSystem : public System {
    Pool<InputComponent> *_ic;

public:

    explicit InputSystem(Pool<InputComponent> & ic) : _ic(&ic) {};

    int update(std::queue<GLint> eventStream);
};


class MovementSystem : public System {
    Pool<TransformComponent> *_tc;
    Pool<InputComponent> *_ic;

public:
    MovementSystem(Pool<TransformComponent> &tc, Pool<InputComponent> &ic) :
        _tc(&tc), _ic(&ic) {};

    void update() override;
};