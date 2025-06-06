#pragma once

#include <vector>
#include <string>
#include <optional>
#include "common.h"
#include "entity.h"
#include "databases.h"

typedef std::vector<WallId> WallVec;
typedef std::vector<GroundId> GroundVec;
typedef std::vector<CardinalDirection> Path;

const unsigned int MAX_SIZE = 100;

enum MoveResult {
	SUCCESS,
	FAIL_WALL,
	FAIL_OCCUPIED
};

class Labyrinth {
public:
	Labyrinth(int x_size, int y_size, Databases* db=nullptr);
	Labyrinth(int x_size, int y_size, WallVec initWalls, GroundVec initGround,
		      int pov_x_init = 0, int pov_y_init = 0, CardinalDirection pov_direction_init = NORTH);

	std::string getName() const { return name; }
	void setName(const std::string& name_) { name = name_; }

	bool setPov(int x, int y, CardinalDirection direction);
	int getPovX() const { return pov_x; }
	int getPovY() const { return pov_y; }
	CardinalDirection getPovDirection() const { return pov_direction; }

	int getSizeX() const { return x_size; }
	int getSizeY() const { return y_size; }
	void resize(int size_x, int size_y);

	GroundId getGroundAbs(int x, int y) const;
	WallId getWallAbs(int x, int y, WallOrientation d) const;
	WallId getWallCardinal(int x, int y, CardinalDirection d) const;

	bool addWall(int x_offset, int y_offset, WallOrientation d, WallId id=0);
	bool removeWall(int x_offset, int y_offset, WallOrientation d);
	void setGround(int x, int y, GroundId id = 0);

	bool writeToFile(const std::string& filename) const;
	bool loadFromFile(const std::string& filename);

	EntityManager& getEntityManager() { return entities; }
	const EntityManager& getEntityManager() const { return entities; }

	bool canMove(int from_x, int from_y, CardinalDirection d) const;
	std::optional<Path> findPath(int from_x, int from_y, int to_x, int to_y) const;
	bool hasLOS(int from_x, int from_y, int to_x, int to_y) const;

	void fillGround(GroundId id);
	static Labyrinth buildSquareLabyrinth(int size, Databases* db=nullptr);
	static Labyrinth buildTriangleLabyrinth(int size, Databases* db = nullptr);
private:
	static int vectorSizeFromGridSize(int x_size, int y_size);

	bool setWall(int x, int y, WallOrientation d, WallId id);
	bool slopeInterceptLOS(int ind1, int dep1, int ind2, int dep2, CardinalDirection d) const;

	std::string name;
	int x_size = 0;
	int y_size = 0;
	WallVec walls = {};
	GroundVec ground = {};
	int pov_x = 0;
	int pov_y = 0;
	CardinalDirection pov_direction = CardinalDirection::NORTH;

	EntityManager entities;
};
