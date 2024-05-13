//
// Created by thomas on 4/18/24.
//

#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__

#include "system.h"
#include <typeindex>

class SystemManager {
public:

    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
    SystemManager() = default;
    ~SystemManager() = default;

    template<typename T, typename...Args>
    void addSystem(Args&&... args) {
        if (systems.find(typeid(T)) != systems.end()) {
            std::cerr << "System already exists" << std::endl;
            return;
        }
        auto typeId = std::type_index(typeid(T));
        systems[typeId] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    T* getSystem() {
        auto typeId = std::type_index(typeid(T));
        if (systems.find(typeId) == systems.end()) {
            std::cerr << "System not found" << std::endl;
            return nullptr;
        }
        return static_cast<T*>(systems[typeId].get());
    }

    template<typename T>
    void update() {
        auto typeId = std::type_index(typeid(T));
        if (systems.find(typeId) == systems.end()) {
            std::cerr << "System not found" << std::endl;
            return;
        }
        systems[typeId]->update();
    }

};

#endif //__SYSTEM_MANAGER_H__
