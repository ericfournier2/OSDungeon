#pragma once
#include <SFML/Graphics.hpp>
#include "common.h"
#include "databases.h"

class Labyrinth;

typedef int EntityId;
struct EntityInfo {
	EntityId id;
	EntityTemplateId template_id;
	int x = 0;
	int y = 0;
	CardinalDirection direction = CardinalDirection::NORTH;
};

class Entity {
public:
	Entity(const EntityInfo& info, const EntityTemplateDb& template_db);
	EntityId getId() const { return info.id; }
	EntityTemplateId getTemplateId() const { return info.template_id; }
	int getX() const { return info.x; }
	int getY() const { return info.y; }
	float getXOffset() const { return getTemplate().x_offset; }
	float getYOffset() const { return getTemplate().y_offset; }
	float getXSize() const { return getTemplate().x_size;; }
	float getYSize() const { return getTemplate().y_size; }
	//void setLabyrinth(Labyrinth* labyrinth_init) { labyrinth = labyrinth_init; }
	TextureId getTexture() { return getTemplate().texture; }
protected:
	EntityTemplateInfo getTemplate() const { return template_db.getElement(info.template_id); }
	EntityInfo info;
	Labyrinth* labyrinth = nullptr;
	const EntityTemplateDb& template_db;
};