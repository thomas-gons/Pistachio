//
// Created by thomas on 17/11/23.
//

#pragma once

#include "common.h"
#include "model.h"


class ModelManager {
    std::unordered_map<std::string, Model> models;

public:

    void loadAllModels() {
        // iterate over all json files in resources/config
        for (const auto& entry : std::filesystem::directory_iterator("resources/config")) {
            std::string path = entry.path();
            if (path.find(".json") == std::string::npos) {
                continue;
            }
            // take the base name of the file without the extension
            std::string modelName = entry.path().stem();
            loadModel(modelName);
        }
    }

    void loadModel(const std::string& modelTag) {
        std::string modelPath = "resources/config/" + modelTag + ".json";
        models[modelTag] = Model(modelPath, modelTag);
    }

    Model getModel(const std::string& modelTag) {
        return models[modelTag];
    }
};