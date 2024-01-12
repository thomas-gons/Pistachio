//
// Created by thomas on 08/10/23.
//
#pragma once

#include <SDL_events.h>
#include <SDL_timer.h>

#include "component.h"
#include "core/system.h"


class GraphicsSystem : public System {
    Pool<GraphicsComponent> *_gc;
    Pool<TransformComponent> *_tc;
    SDL_Renderer *renderer;

public:
    GraphicsSystem(Pool<GraphicsComponent> &gc, Pool<TransformComponent> &tc, SDL_Renderer *renderer) :
        _gc(&gc), _tc(&tc), renderer(renderer) {};

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