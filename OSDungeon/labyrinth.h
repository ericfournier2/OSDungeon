#pragma once

#include <vector>
#include <string>
#include "common.h"
#include "entity.h"
#include "databases.h"

typedef std::vector<WallTypeId> WallVec;
typedef std::vector<GroundTypeId> GroundVec;
typedef std::vector<Entity> EntityVec;
typedef std::map<EntityId, Entity> EntityMap;
const unsigned int MAX_SIZE = 100;

enum MoveResult {
	SUCCESS,
	FAIL_WALL,
	FAIL_OCCUPIED
};

class Labyrinth {
public:
	Labyrinth(int x_size, int y_size);
	Labyrinth(int x_size, int y_size, WallVec initWalls, GroundVec initGround,
		      int pov_x_init = 0, int pov_y_init = 0, CardinalDirection pov_direction_init = NORTH);

	int getPovX() const { return pov_x; }
	int getPovY() const { return pov_y; }
	CardinalDirection getPovDirection() const { return pov_direction; }

	int getSizeX() const { return x_size; }
	int getSizeY() const { return y_size; }

	GroundTypeId getGroundAbs(int x, int y) const;
	WallTypeId getWallAbs(int x, int y, WallOrientation d) const;

	GroundTypeId getGroundRel(int x_offset, int y_offset) const;
	WallTypeId getWallRel(int x_offset, int y_offset, RelativeDirection direction) const;
	
	bool addWall(int x_offset, int y_offset, WallOrientation d, WallTypeId id=0);
	bool removeWall(int x_offset, int y_offset, WallOrientation d);
	void setGround(int x, int y, GroundTypeId id = 0);

	bool setPov(int x, int y, CardinalDirection direction);
	MoveResult movePovRel(int x_offset, int y_offset);
	void turnPovRel(RelativeDirection direction);

	MoveResult advance();
	MoveResult moveBack();

	bool writeToFile(const std::string& filename) const;
	bool loadFromFile(const std::string& filename);

	EntityId addEntity(Entity entity);
	void removeEntity(EntityId id);
	EntityId addEntityFromTemplate(EntityTemplateId template_id, int x, int y, CardinalDirection d, const EntityTemplateDb& template_db);
	Entity getEntity(EntityId id, const EntityTemplateDb& template_db) const;
	EntityVec getEntityAbs(int x, int y) const;
	EntityVec getEntityRel(int x, int y) const;
	const EntityMap& getAllEntities() const { return entities; }

	std::string printToString() const;
private:
	static int vectorSizeFromGridSize(int x_size, int y_size);

	int getAbsXFromPovX(int x_offset, int y_offset) const;
	int getAbsYFromPovY(int x_offset, int y_offset) const;
	CardinalDirection getAbsDirectionFromRelativeDirection(RelativeDirection direction) const;

	bool setWall(int x, int y, WallOrientation d, WallTypeId id);

	std::string printXLineToString(unsigned int y) const;
	std::string printGroundTileToString(unsigned int x, unsigned int y) const;
	std::string printYLineToString(unsigned int y) const;

	int x_size = 0;
	int y_size = 0;
	WallVec walls = {};
	GroundVec ground = {};
	int pov_x = 0;
	int pov_y = 0;
	CardinalDirection pov_direction = CardinalDirection::NORTH;

	EntityMap entities;
};
