#include "labyrinth.h"
#include <cassert>
#include <fstream>
#include <queue>
#include <set>

bool Labyrinth::setWall(int x, int y, WallOrientation d, WallTypeId id)
{
	// Determine if it is out of bound
	if (x < 0 || y < 0 || x >(int) x_size || y >(int) y_size || (x == x_size && d == HORIZONTAL) || (y == y_size && d == VERTICAL)) {
		return false;
	}
	else if (d == HORIZONTAL) {
		walls[x + y * x_size] = id;
		return true;
	} else {
		unsigned int vertOffset = (y_size + 1) * x_size;
		walls[vertOffset + y + x * y_size] = id;
		return true;
	}

	// Never reached.
	return false;
}

bool Labyrinth::addWall(int x, int y, WallOrientation d, WallTypeId id) {
	return setWall(x, y, d, id);
}

bool Labyrinth::removeWall(int x, int y, WallOrientation d)
{
	return setWall(x, y, d, 0);
}

void Labyrinth::setGround(int x, int y, GroundTypeId id)
{
	ground[y * x_size + x] = id;
}

bool Labyrinth::setPov(int x, int y, CardinalDirection direction)
{
	if (x >= x_size || y >= y_size || x < 0 || y < 0)
		return false;

	pov_x = x;
	pov_y = y;
	pov_direction = direction;

	return true;
}

int Labyrinth::vectorSizeFromGridSize(int x_size, int y_size) {
	return (x_size + 1) * y_size + (y_size + 1) * x_size;
}

Labyrinth::Labyrinth(int x_size_init, int y_size_init)
{
	x_size = x_size_init;
	if (x_size > MAX_SIZE)
		x_size = MAX_SIZE;

	if (x_size < 0)
		x_size = 0;

	y_size = y_size_init;
	if (y_size > MAX_SIZE)
		y_size = MAX_SIZE;
	
	if (y_size < 0)
		y_size = 0;


	walls = WallVec(vectorSizeFromGridSize(x_size, y_size), 0);
	ground = GroundVec(x_size * y_size, 0);
}

Labyrinth::Labyrinth(int x_size_init, int y_size_init, WallVec initWalls, GroundVec initGround,
	                 int pov_x_init, int pov_y_init, CardinalDirection pov_direction_init)
 : x_size(x_size_init),
   y_size(y_size_init),
   walls(initWalls),
   ground(initGround),
   pov_x(pov_x_init),
   pov_y(pov_y_init),
   pov_direction(pov_direction_init)
{
	// Sanitize our inputs.
	if (x_size > MAX_SIZE || y_size > MAX_SIZE || x_size < 0 || y_size < 0 ||
		walls.size() != vectorSizeFromGridSize(x_size, y_size) ||
		ground.size() != x_size * y_size) {
		x_size = 0;
		y_size = 0;
		walls = {};
		ground = {};
	}

	if (pov_x < 0 || pov_x > x_size || pov_y < 0 || pov_y > y_size) {
		pov_x = 0;
		pov_y = 0;
	}

	assert(walls.size() == vectorSizeFromGridSize(x_size_init, y_size_init));
	assert(ground.size() == x_size * y_size);
}

WallTypeId Labyrinth::getWallAbs(int x, int y, WallOrientation d) const{
	// Determine if it is out of bound
	if (x < 0 || y < 0 || x > (int) x_size || y > (int) y_size || (x == x_size && d == HORIZONTAL) || (y == y_size && d == VERTICAL)) {
		return 0;
	}
	
	if (d == HORIZONTAL) {
		return walls[x + y * x_size];
	}
	else {
		unsigned int vertOffset = (y_size + 1) * x_size;
		return walls[vertOffset + y + x * y_size];
	}
}

WallTypeId Labyrinth::getWallCardinal(int x, int y, CardinalDirection d) const {
	switch (d) {
	case NORTH:
		return getWallAbs(x, y + 1, WallOrientation::HORIZONTAL);
		break;
	case SOUTH:
		return getWallAbs(x, y, WallOrientation::HORIZONTAL);
		break;
	case EAST:
		return getWallAbs(x + 1, y, WallOrientation::VERTICAL);
		break;
	case WEST:
		return getWallAbs(x, y, WallOrientation::VERTICAL);
		break;
	}

	// Shouldn't ever be reached.
	return 0;
}

GroundTypeId Labyrinth::getGroundAbs(int x, int y) const
{
	if (x < 0 || y < 0 || x >= (int) x_size || y >= (int) y_size) {
		return 0;
	}

	return ground[y * x_size + x];
}

bool Labyrinth::canMove(int from_x, int from_y, CardinalDirection d) const
{
	return getWallCardinal(from_x, from_y, d) == 0;
}

struct PathStep {
	Coord c;
	Path p;
};

typedef std::queue<PathStep> PathStepQueue;
typedef std::set<Coord> CoordSet;

void searchDirection(const Labyrinth& labyrinth, const PathStep& ps, CardinalDirection d, PathStepQueue& work_queue, CoordSet& traversed) {
	int x_offset = 0;
	int y_offset = 0;
	switch (d) {
	case CardinalDirection::NORTH:
		y_offset = 1;
		break;
	case CardinalDirection::SOUTH:
		y_offset = -1;
		break;
	case CardinalDirection::EAST:
		x_offset = 1;
		break;
	case CardinalDirection::WEST:
		x_offset = -1;
		break;
	}

	if (labyrinth.canMove(ps.c.x, ps.c.y, d)) {
		Coord next_coord({ ps.c.x + x_offset, ps.c.y + y_offset });
		if (!traversed.contains(next_coord)) {
			Path next_path = ps.p;
			next_path.push_back(d);
			work_queue.push({ next_coord, next_path });
		}
	}
}

// Breadth-first path finding.
std::optional<Path> Labyrinth::findPath(int from_x, int from_y, int to_x, int to_y) const
{
	CoordSet traversed;
	PathStepQueue work_queue;
	Coord init_coord({ from_x, from_y });
	Coord target_coord({ to_x, to_y });

	// Initialize the queue with the start coordinate.
	work_queue.push({ init_coord, Path() });
	while (!work_queue.empty()) {
		PathStep current = work_queue.front();
		work_queue.pop();
		traversed.insert(current.c);

		if (current.c == target_coord) {
			return current.p;
		} else {
			searchDirection(*this, current, CardinalDirection::NORTH, work_queue, traversed);
			searchDirection(*this, current, CardinalDirection::EAST, work_queue, traversed);
			searchDirection(*this, current, CardinalDirection::SOUTH, work_queue, traversed);
			searchDirection(*this, current, CardinalDirection::WEST, work_queue, traversed);
		}
	}

	// If there is no path, return the empty path.
	return std::nullopt;
}

const char file_identifier[] = "OSDUNGEON0.0";

bool Labyrinth::writeToFile(const std::string& filename) const {
	std::ofstream stream;
	stream.open(filename, std::ofstream::out | std::ofstream::binary);
	if (!stream.fail()) {
		stream << file_identifier << '\0';
		stream.write(reinterpret_cast<const char*>(&x_size), sizeof(x_size));
		stream.write(reinterpret_cast<const char*>(&y_size), sizeof(y_size));
		stream.write(reinterpret_cast<const char*>(walls.data()), walls.size() * sizeof(WallTypeId));
		stream.write(reinterpret_cast<const char*>(ground.data()), ground.size() * sizeof(GroundTypeId));
		entities.writeToStream(stream);
	}

	return !stream.fail();
}

bool Labyrinth::loadFromFile(const std::string& filename) {
	std::ifstream stream;
	stream.open(filename, std::ifstream::in | std::ifstream::binary);
	if (!stream.fail()) {
		char id_string[sizeof(file_identifier)];
		stream.read(id_string, sizeof(id_string));
		if (strcmp(file_identifier, id_string) == 0) {
			unsigned int x_size_read = 0;
			unsigned int y_size_read = 0;
			stream.read(reinterpret_cast<char*>(&x_size_read), sizeof(x_size_read));
			stream.read(reinterpret_cast<char*>(&y_size_read), sizeof(y_size_read));
			if (x_size_read < MAX_SIZE && y_size_read < MAX_SIZE) {
				WallVec walls_read = WallVec(vectorSizeFromGridSize(x_size_read, y_size_read), 0);
				GroundVec ground_read = GroundVec(x_size_read * y_size_read, 0);

				stream.read(reinterpret_cast<char*>(walls_read.data()), walls_read.size() * sizeof(WallTypeId));
				stream.read(reinterpret_cast<char*>(ground_read.data()), ground_read.size() * sizeof(GroundTypeId));
				entities.readFromStream(stream);

				if (!stream.fail()) {
					x_size = x_size_read;
					y_size = y_size_read;
					walls = walls_read;
					ground = ground_read;
					pov_x = 0;
					pov_y = 0;
					pov_direction = CardinalDirection::NORTH;
					return true;
				}
			}
		}
	}

	return false;
}
