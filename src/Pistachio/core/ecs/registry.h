//
// Created by thomas on 01/09/23.
//
#ifndef __REGISTRY_H__
#define __REGISTRY_H__


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
 * It holds all the mEntities, components and their relationships.
 * It also provides methods to create, destroy and assign components to mEntities.
 * @tparam MAX_ENTITIES the maximum number of mEntities that can be created
 * @tparam lastEntityUUID the last entity UUID that was created
 * @tparam entityCount the number of mEntities that were created
 * @tparam mEntities an array of entity UUIDs
 * @tparam destroyedEntities a set of all the mEntities that were destroyed
 * @tparam entityComponents a map of all the components that were assigned to an entity.
 * @tparam pools a map of all the component pools.
 */
class Registry {

    std::uint32_t _mLastEntityUUID = 0;
    std::uint32_t _mEntityCount = 0;
    Pool<uint32_t> _mEntities = {};
    std::unordered_set<Entity> _mDestroyedEntities = {};

    std::unordered_map<Entity, std::unordered_map<ComponentHash, TComponentUUID>> _mEntityToComponents = {};
    std::unordered_map<ComponentHash, std::unordered_map<TComponentUUID , Entity>> _mComponentToEntities = {};
    std::unordered_map<ComponentHash, Pool<std::any>> _mPools;

    ModelManager _mModelManager = ModelManager();

public:

    Registry() {
//        modelManager.loadAllModels();
    }

    /**
     * Creates a new entity and assigns it a UUID.
     * @return the UUID of the entity
     */
    std::uint32_t create() {
        if (_mEntityCount == MAX_ENTITIES) {
            std::cerr << "Maximum number of entities reached" << std::endl;
            return std::numeric_limits<std::uint32_t>::max();
        }
        _mEntities.add(_mLastEntityUUID);
        _mEntityToComponents[_mLastEntityUUID] = {};
        _mLastEntityUUID++;
        _mEntityCount++;
        return _mLastEntityUUID - 1;
    }

    /**
     * Destroys an entity and removes all its components.
     * @param entityUUID the UUID of the entity to destroy
     */
    void destroy(Entity entityUUID) {
        if (!entityCheck(entityUUID)) return;

        // remove all mapping of the entity to its components and the components themselves
        for (auto &component : _mEntityToComponents.at(entityUUID)) {
            auto typeHash = component.first;
            auto componentUUID = component.second;
            _mComponentToEntities[typeHash].erase(componentUUID);
            _mPools[typeHash].remove(componentUUID);
        }

        _mEntityToComponents.erase(entityUUID);
        _mEntities.remove(entityUUID);
        _mEntityCount--;
        _mDestroyedEntities.insert(entityUUID);
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
        if (_mPools.find(typeHash) == _mPools.end()) {
            _mPools[typeHash] = Pool<std::any>();
        }
        if (_mComponentToEntities.find(typeHash) == _mComponentToEntities.end()) {
            _mComponentToEntities[typeHash] = {};
        }

        if (_mEntityToComponents[entityUUID].find(typeHash) != _mEntityToComponents[entityUUID].end()) {
            std::cerr << "Entity " << entityUUID << " already has a component of type "
                      << componentNames[typeHash] << std::endl;
            return;
        }
        // retrieve the component type pool and converts it to add the component to it
        auto pool = &_mPools[typeHash];
        pool->add(component);
        // add the component to the entity with its index in the pool and its type hash
        _mEntityToComponents[entityUUID][typeHash] = pool->size() - 1;
        _mComponentToEntities[typeHash][pool->size() - 1] = entityUUID;
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
            for (auto component : _mPools[typeHash]) {
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
        auto componentUUID = _mEntityToComponents[entityUUID][typeHash];
        _mPools[typeHash].remove(componentUUID);
        _mEntityToComponents[entityUUID].erase(typeHash);
        _mComponentToEntities[typeHash].erase(componentUUID);
    }

    /**
     * Instantiates an entity with all its components based on a model.
     * @param modelName the name of the model
     * @return the UUID of the entity
     */
    std::uint32_t createFromModel(const std::string& modelName) {
        auto model = _mModelManager.getModel(modelName);
        this->create();
        // iterate through the constructor of the model
        for (auto& [componentName, constructor] : model.getComponentConstructors()) {
            // call the constructor and assign the component to the entity
            auto component = constructor();
            if (componentName == "Transform") {
                auto tc = std::dynamic_pointer_cast<TransformComponent>(component);
                this->assign<TransformComponent>(_mLastEntityUUID - 1, *tc);
            } else if (componentName == "Graphics") {
                auto gc = std::dynamic_pointer_cast<GraphicsComponent>(component);
                this->assign<GraphicsComponent>(_mLastEntityUUID - 1, *gc);
            }
        }
        return _mLastEntityUUID - 1;
    }

private:

    /**
     * Checks if an entity exists and has not been destroyed.
     * @param entityUUID the UUID of the entity
     * @return true if the entity exists and has not been destroyed, false otherwise
     */
    bool entityCheck(Entity entityUUID) {
        if (_mEntities.empty()) {
            std::cerr << "There is no entity" << std::endl;
            return false;
        } else if (entityUUID > _mEntityCount) {
            std::cerr << "Entity " << entityUUID << " does not exist" << std::endl;
            return false;
        } else if (_mDestroyedEntities.find(entityUUID) != _mDestroyedEntities.end()) {
            std::cerr << "Entity " << entityUUID << " has already been destroyed" << std::endl;
            return false;
        }
        return true;
    }
};

#endif //__REGISTRY_H__