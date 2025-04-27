#pragma once
#include <SFML/Graphics.hpp>
#include "common.h"
#include "databases.h"
#include "shallow_entities.h"
#include "labyrinth.h"
#include "game_state.h"

class Labyrinth;


class Entity {
public:
	Entity(ShallowEntity& info, const EntityTemplateDb& template_db);
	operator ShallowEntity() const { return info; }

	ShallowEntity getShallowEntity() const { return info; }
	EntityId getId() const { return info.id; }
	EntityTemplateId getTemplateId() const { return info.template_id; }
	int getX() const { return info.x; }
	int getY() const { return info.y; }
	float getXOffset() const { return getTemplate().x_offset; }
	float getYOffset() const { return getTemplate().y_offset; }
	float getXSize() const { return getTemplate().x_size;; }
	float getYSize() const { return getTemplate().y_size; }
	MovementType getMovementType() const { return getTemplate().movement; }
	CollisionType getCollisionType() const { return getTemplate().collision; }
	InteractionType getInteractionType() const { return getTemplate().interaction; }
	//void setLabyrinth(Labyrinth* labyrinth_init) { labyrinth = labyrinth_init; }
	TextureId getTexture() const { return getTemplate().texture; }
	const TileVec& getTiles(CardinalDirection d) const { return getTemplate().getTileVec(d); };

	void move(const Labyrinth& labyrinth, GameState& state);
	bool collide(const Labyrinth& labyrinth, GameState& state);
protected:
	EntityTemplateInfo getTemplate() const { return template_db.getElement(info.template_id); }
	bool moveCardinal(const Labyrinth& labyrinth, CardinalDirection d);

	ShallowEntity& info;
	//Labyrinth* labyrinth = nullptr;
	const EntityTemplateDb& template_db;
};

// Manages a set of entities inside a labyrinth.
/*class EntityManager {
public:
	EntityId addEntity(EntityInfo entity);
	void removeEntity(EntityId id);
	EntityId addEntityFromTemplate(EntityTemplateId template_id, int x, int y, CardinalDirection d, const EntityTemplateDb& template_db);
	Entity getEntity(EntityId id, const EntityTemplateDb& template_db) const;
	EntityVec getEntityAbs(int x, int y) const;
	EntityVec getEntityRel(int x, int y) const;
	const EntityMap& getAllEntities() const { return entities; }

private:
	const Labyrinth& labyrinth;
	EntityMap entities;

};*/