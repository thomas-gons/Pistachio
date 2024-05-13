//
// Created by thomas on 17/11/23.
//

#ifndef __MODEL_MANAGER_H__
#define __MODEL_MANAGER_H__

#include "common.h"
#include "model.h"


class ModelManager {
    std::unordered_map<std::string, Model*> _mModels;

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
        _mModels[modelTag] = new Model(modelPath, modelTag);
    }

    Model getModel(const std::string& modelTag) {
        return *_mModels[modelTag];
    }
};

#endif //__MODEL_MANAGER_H__