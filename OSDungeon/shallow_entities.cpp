#include "shallow_entities.h"

EntityId EntityStateManager::findFreeId() const {
	// Get first free ID
	EntityId id = 1;
	while (entities.contains(id)) {
		++id;
	}

	return id;
}

EntityId EntityStateManager::addEntity(const EntityState& entity) {
	if (entities.contains(entity.id)) {
		EntityId new_id = findFreeId();
		EntityState new_info = entity;
		new_info.id = new_id;
		entities.emplace(new_id, new_info);
		return new_id;
	} else {
		entities.emplace(entity.id, entity);
		return entity.id;
	}
}

EntityId EntityStateManager::addEntity(EntityTemplateId template_id, int x, int y, CardinalDirection d) {
	EntityId id = findFreeId();
	entities.emplace(id, EntityState(id, template_id, x, y, d));
	return id;
}

bool EntityStateManager::updateEntity(const EntityState& entity) {
	if (entities.contains(entity.id)) {
		entities[entity.id] = entity;
		return true;
	}

	return false;
}

void EntityStateManager::removeEntity(EntityId id) {
	entities.erase(id);
}

EntityState EntityStateManager::getEntity(EntityId id) const {
	if (entities.contains(id)) {
		return entities.at(id);
	} else {
		return EntityState();
	}
}

EntityStateVec EntityStateManager::getEntityAbs(int x, int y) const {
	EntityStateVec retval;
	for (auto const& [key, val] : entities) {
		if (val.x == x && val.y == y) {
			retval.push_back(val);
		}
	}

	return retval;
}

bool EntityStateManager::writeToStream(std::ofstream& stream) const {
	auto db_size = entities.size();
	stream.write(reinterpret_cast<char*>(&db_size), sizeof(db_size));
	for (const auto& [key, value] : entities) {
		stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
	}
	return !stream.fail();
}

bool EntityStateManager::readFromStream(std::ifstream& stream) {
	entities.clear();

	auto db_size = entities.size();
	stream.read(reinterpret_cast<char*>(&db_size), sizeof(db_size));
	for (int i = 0; i < db_size; ++i) {
		EntityState info;
		stream.read(reinterpret_cast<char*>(&info), sizeof(EntityState));
		entities.emplace(info.id, info);
	}
	return !stream.fail();
}

