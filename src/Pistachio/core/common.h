//
// Created by thomas on 04/09/23.
//

#pragma once

#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <queue>
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>


#define MAX_ENTITIES 200000

using json = nlohmann::json;

template<typename T, typename...Args>
T *createFromJson(const json& jsonObject, const Args&... remainingAttributes) {
    return new T(jsonObject[remainingAttributes]...);
}
