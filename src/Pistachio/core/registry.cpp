//
// Created by thomas on 12/01/24.
//
#include "registry.h"

Registry::Registry() {
    modelManager.loadAllModels();
}

void Registry::create() {
    if (entityCount == MAX_ENTITIES) {
            std::cerr << "Maximum number of entities reached" << std::endl;
            return;
        }
        entities.add(lastEntityUUID);
        entityToComponents[lastEntityUUID] = {};
        lastEntityUUID++;
        entityCount++;
}

void Registry::destroy(Entity entityUUID) {
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


