#pragma once
#include <SFML/Graphics.hpp>
#include "common.h"
#include "databases.h"
#include "entity_state.h"
#include "game_state.h"

class Labyrinth;

class Entity {
public:
	Entity();
	Entity(const EntityState& info) : entity_state(info) {}
	Entity(const EntityState& info, const Databases& db);
	
	operator EntityState() const { return entity_state; }
	EntityState getState() const { return entity_state; }

	operator EntityTemplateInfo() const { return getTemplate(); }
	EntityTemplateInfo getTemplate() const { return template_info; }

	operator SpriteInfo() const { return sprite_info; }
	SpriteInfo getSpriteInfo() const { return sprite_info; }

	EntityId getId() const { return entity_state.id; }
	EntityTemplateId getTemplateId() const { return entity_state.template_id; }
	int getX() const { return entity_state.x; }
	int getY() const { return entity_state.y; }
	CardinalDirection getDirection() const { return entity_state.direction;  }

	float getXSize() const { return getTemplate().x_size; }
	float getYSize() const { return getTemplate().y_size; }
	MovementType getMovementType() const { return getTemplate().movement; }
	CollisionType getCollisionType() const { return getTemplate().collision; }
	InteractionType getInteractionType() const { return getTemplate().interaction; }
	
	TextureId getTexture() const { return getSpriteInfo().texture; }
	TileVec getTiles(RelativeDirection d) const { return getSpriteInfo().getTileVec(d); };

	sf::Sprite getSprite(RelativeDirection d) const;

	void refresh(const Databases& db);
	void update(Databases& db) const;

	void move(const Labyrinth& labyrinth, GameState& state);
	bool collide(const Labyrinth& labyrinth, GameState& state);
	void clicked(GameState& state);
protected:
	SpriteInfo getSprite() const { return sprite_info; }
	bool moveCardinal(const Labyrinth& labyrinth, CardinalDirection d);

	EntityState entity_state;
	EntityTemplateInfo template_info;
	SpriteInfo sprite_info;
	TextureInfo texture;
	sf::Clock animation_clock;
};

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<EntityId, std::shared_ptr<Entity>> EntityMap;

// Manages a list of shallow entities used to initialize a labyrinth.
class EntityManager {
public:
	EntityManager(Databases* db_ = nullptr) : db(db_) {}
	EntityId addEntity(const EntityState& entity);
	EntityId addEntity(EntityTemplateId template_id, int x, int y, CardinalDirection d);
	bool updateEntity(const Entity& entity);
	void removeEntity(EntityId id);

	std::shared_ptr<Entity> getEntity(EntityId id) const;
	EntityVec getEntityAbs(int x, int y) const;
	EntityMap& getAllEntities() { return entities; }
	const EntityMap& getAllEntities() const { return entities; }

	bool writeToStream(std::ofstream& stream) const;
	bool readFromStream(std::ifstream& stream);
private:
	EntityId findFreeId() const;

	Databases* db;
	EntityMap entities;
};
