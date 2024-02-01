//
// Created by thomas on 01/02/24.
//
#include "core/registry.h"

int main() {
    Registry registry;
    std::uint32_t uuid = registry.create();

    TransformComponent tc = TransformComponent(0, 0);
    registry.assign(uuid, tc);

    GraphicsComponent gc = GraphicsComponent("flame", nullptr, nullptr);
    registry.assign(uuid, gc);

    Pool<TransformComponent> tcPool = registry.getPool<TransformComponent>();
    std::cout << tcPool << std::endl;

    Pool<GraphicsComponent> gcPool = registry.getPool<GraphicsComponent>();
    std::cout << gcPool << std::endl;
    return 0;
}