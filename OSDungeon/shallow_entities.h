#pragma once
#include <vector>
#include <map>

#include "common.h"
#include "databases.h"

typedef int EntityId;
// Shallow entities are plain data structures used for keeping track of, saving and loading
// initial entities within a labyrinth.
struct EntityState {
	EntityId id = 0;
	EntityTemplateId template_id = 0;
	int x = 0;
	int y = 0;
	CardinalDirection direction = CardinalDirection::NORTH;

	bool fixed_position = false;
	float x_sub = 0.5f;
	float y_sub = 0.5f;

	EntityState() {}
	EntityState(EntityId id_init, EntityTemplateId template_id_init, int x_init, int y_init, CardinalDirection direction_init, bool fixed = false, float x_sub_init = 0.5f, float y_sub_init = 0.5f)
		: id(id_init), template_id(template_id_init), x(x_init), y(y_init), direction(direction_init), fixed_position(fixed), x_sub(x_sub_init), y_sub(y_sub_init) {}
};

typedef std::vector<EntityState> EntityStateVec;
typedef std::map<EntityId, EntityState> EntityStateMap;

// Manages a list of shallow entities used to initialize a labyrinth.
class EntityStateManager {
public:
	EntityId addEntity(const EntityState& entity);
	EntityId addEntity(EntityTemplateId template_id, int x, int y, CardinalDirection d);
	bool updateEntity(const EntityState& entity);
	void removeEntity(EntityId id);

	EntityState getEntity(EntityId id) const;
	EntityStateVec getEntityAbs(int x, int y) const;
	EntityStateMap& getAllEntities() { return entities; }
	const EntityStateMap& getAllEntities() const { return entities; }

	bool writeToStream(std::ofstream& stream) const;
	bool readFromStream(std::ifstream& stream);
private:
	EntityId findFreeId() const;

	EntityStateMap entities;
};
