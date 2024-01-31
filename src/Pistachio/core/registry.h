//
// Created by thomas on 01/09/23.
//

#pragma once

#include <typeindex>
#include <unordered_set>
#include <any>

#include "common.h"
#include "component.h"
#include "pool.h"
#include "model/model.h"
#include "model/modelManager.h"


// entity is as a UUID on 32 bits
using Entity = std::uint32_t;


/**
 * The registry is the central point of the ECS architecture.
 * It holds all the entities, components and their relationships.
 * It also provides methods to create, destroy and assign components to entities.
 * @tparam MAX_ENTITIES the maximum number of entities that can be created
 * @tparam lastEntityUUID the last entity UUID that was created
 * @tparam entityCount the number of entities that were created
 * @tparam entities an array of entity UUIDs
 * @tparam destroyedEntities a set of all the entities that were destroyed
 * @tparam entityComponents a map of all the components that were assigned to an entity.
 * @tparam pools a map of all the component pools.
 */
class Registry {

    std::uint32_t lastEntityUUID = 0;
    std::uint32_t entityCount = 0;
    Pool<uint32_t> entities = {};
    std::unordered_set<Entity> destroyedEntities = {};

    std::unordered_map<Entity, std::unordered_map<ComponentHash, TComponentUUID>> entityToComponents = {};
    std::unordered_map<ComponentHash, std::unordered_map<TComponentUUID , Entity>> componentToEntities = {};
    std::unordered_map<ComponentHash, Pool<std::any>> pools;

    ModelManager modelManager = ModelManager();

public:

    Registry() {
//        modelManager.loadAllModels();
    }

    /**
     * Creates a new entity and assigns it a UUID.
     */
    void create() {
        if (entityCount == MAX_ENTITIES) {
            std::cerr << "Maximum number of entities reached" << std::endl;
            return;
        }
        entities.add(lastEntityUUID);
        entityToComponents[lastEntityUUID] = {};
        lastEntityUUID++;
        entityCount++;
    }

    /**
     * Destroys an entity and removes all its components.
     * @param entityUUID the UUID of the entity to destroy
     */
    void destroy(Entity entityUUID) {
        if (!entityCheck(entityUUID)) return;

        // remove all mapping of the entity to its components and the components themselves
        for (auto &component : entityToComponents.at(entityUUID)) {
            auto typeHash = component.first;
            auto componentUUID = component.second;
            componentToEntities[typeHash].erase(componentUUID);
            pools[typeHash].remove(componentUUID);
        }

        entityToComponents.erase(entityUUID);
        entities.remove(entityUUID);
        entityCount--;
        destroyedEntities.insert(entityUUID);
    }

    /**
     * Assigns a component to an entity.
     * @tparam U the type of the component
     * @param entityUUID the UUID of the entity
     * @param component the component to assign
     */
    template<typename U>
    void assign(Entity entityUUID, U component) {
        if (!entityCheck(entityUUID)) return;

        auto typeHash = typeid(U).hash_code();
        if (pools.find(typeHash) == pools.end()) {
            pools[typeHash] = Pool<std::any>();
        }
        if (componentToEntities.find(typeHash) == componentToEntities.end()) {
            componentToEntities[typeHash] = {};
        }

        // retrieve the component type pool and converts it to add the component to it
        auto pool = &pools[typeHash];
        pool->add(component);
        // add the component to the entity with its index in the pool and its type hash
        entityToComponents[entityUUID][typeHash] = pool->size() - 1;
        componentToEntities[typeHash][pool->size() - 1] = entityUUID;
    }

    /**
     * Removes a component from an entity.
     * @tparam U the type of the component
     * @return the pool of components of type U
     */
    template<typename U>
    Pool<U> getPool() {
        try {
            auto pool = Pool<U>();
            auto typeHash = typeid(U).hash_code();
            // optimize: potential bottleneck => n cast operations
            for (auto component : pools[typeHash]) {
                pool.add(std::any_cast<U>(component));
            }
            return pool;
        } catch (std::bad_any_cast& e) {
            std::cout << e.what() << std::endl;
            throw std::runtime_error("Could not cast component to the specified type");
        };
    }

    /**
     * Removes a component from an entity.
     * @tparam U the type of the component
     * @param entityUUID the UUID of the entity
     */
    template<typename U>
    void remove(Entity entityUUID) {
        if (!entityCheck(entityUUID)) return;

        auto typeHash = typeid(U).hash_code();
        auto componentUUID = entityToComponents[entityUUID][typeHash];
        pools[typeHash].remove(componentUUID);
        entityToComponents[entityUUID].erase(typeHash);
        componentToEntities[typeHash].erase(componentUUID);
    }


    void createFromModel(const std::string& modelName) {
        auto model = modelManager.getModel(modelName);
        this->create();
        // iterate through the constructor of the model
        for (auto& [componentName, constructor] : model.getComponentConstructors()) {
            // call the constructor and assign the component to the entity
            auto component = constructor();
            if (componentName == "Transform") {
                auto tc = std::dynamic_pointer_cast<TransformComponent>(component);
                this->assign<TransformComponent>(lastEntityUUID - 1, *tc);
            } else if (componentName == "Graphics") {
                auto gc = std::dynamic_pointer_cast<GraphicsComponent>(component);
                this->assign<GraphicsComponent>(lastEntityUUID - 1, *gc);
            }
        }
    }

private:

    bool entityCheck(Entity entityUUID) {
        if (entities.empty()) {
            std::cerr << "There is no entity" << std::endl;
            return false;
        } else if (entityUUID > entityCount) {
            std::cerr << "Entity " << entityUUID << " does not exist" << std::endl;
            return false;
        } else if (destroyedEntities.find(entityUUID) != destroyedEntities.end()) {
            std::cerr << "Entity " << entityUUID << " has already been destroyed" << std::endl;
            return false;
        }
        return true;
    }
};