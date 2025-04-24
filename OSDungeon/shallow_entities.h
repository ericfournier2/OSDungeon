#pragma once
#include <vector>
#include <map>

#include "common.h"
#include "databases.h"

typedef int EntityId;
// Shallow entities are plain data structures used for keeping track of, saving and loading
// initial entities within a labyrinth.
struct ShallowEntity {
	EntityId id = 0;
	EntityTemplateId template_id = 0;
	int x = 0;
	int y = 0;
	CardinalDirection direction = CardinalDirection::NORTH;

	ShallowEntity() {}
	ShallowEntity(EntityId id_init, EntityTemplateId template_id_init, int x_init, int y_init, CardinalDirection direction_init)
		: id(id_init), template_id(template_id_init), x(x_init), y(y_init), direction(direction_init) {}

	float getXOffset(const EntityTemplateDb& edb) const { return getTemplate(edb).x_offset; }
	float getYOffset(const EntityTemplateDb& edb) const { return getTemplate(edb).y_offset; }
	float getXSize(const EntityTemplateDb& edb) const { return getTemplate(edb).x_size;; }
	float getYSize(const EntityTemplateDb& edb) const { return getTemplate(edb).y_size; }
	MovementType getMovementType(const EntityTemplateDb& edb) const { return getTemplate(edb).movement; }
	CollisionType getCollisionType(const EntityTemplateDb& edb) const { return getTemplate(edb).collision; }
	InteractionType getInteractionType(const EntityTemplateDb& edb) const { return getTemplate(edb).interaction; }
	//void setLabyrinth(Labyrinth* labyrinth_init) { labyrinth = labyrinth_init; }
	TextureId getTexture(const EntityTemplateDb& edb) const { return getTemplate(edb).texture; }

	EntityTemplateInfo getTemplate(const EntityTemplateDb& edb) const { return edb.getElement(template_id); }

};

typedef std::vector<ShallowEntity> ShallowEntityVec;
typedef std::map<EntityId, ShallowEntity> ShallowEntityMap;

// Manages a list of shallow entities used to initialize a labyrinth.
class ShallowEntityManager {
public:
	EntityId addEntity(const ShallowEntity& entity);
	EntityId addEntity(EntityTemplateId template_id, int x, int y, CardinalDirection d);
	bool updateEntity(const ShallowEntity& entity);
	void removeEntity(EntityId id);

	ShallowEntity getEntity(EntityId id) const;
	ShallowEntityVec getEntityAbs(int x, int y) const;
	//ShallowEntityVec getEntityRel(int x, int y) const;
	ShallowEntityMap& getAllEntities() { return entities; }
	const ShallowEntityMap& getAllEntities() const { return entities; }

	bool writeToStream(std::ofstream& stream) const;
	bool readFromStream(std::ifstream& stream);
private:
	EntityId findFreeId() const;

	ShallowEntityMap entities;
};
