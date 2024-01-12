//
// Created by thomas on 02/09/23.
//
#include "common.h"
#include <functional>

#pragma once

using TComponentUUID = std::uint32_t;
using ComponentHash = std::size_t;

class Component {
public:
    Component() = default;
    virtual ~Component() = default;
};

using TComponentConstructor = std::function<std::shared_ptr<Component>()>;
