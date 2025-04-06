#include "labyrinth.h"
#include <cassert>
#include <fstream>


int Labyrinth::getAbsXFromPovX(int x_offset, int y_offset) const {
	int final_offset_x = pov_x;

	switch (pov_direction) {
	case NORTH:
		final_offset_x += x_offset;
		break;
	case EAST:
		final_offset_x += y_offset;
		break;
	case SOUTH:
		final_offset_x -= x_offset;
		break;
	case WEST:
		final_offset_x -= y_offset;
		break;
	}

	return final_offset_x;
}

int Labyrinth::getAbsYFromPovY(int x_offset, int y_offset) const {
	int final_offset_y = pov_y;

	switch (pov_direction) {
	case NORTH:
		final_offset_y += y_offset;
		break;
	case EAST:
		final_offset_y -= x_offset;
		break;
	case SOUTH:
		final_offset_y -= y_offset;
		break;
	case WEST:
		final_offset_y += x_offset;
		break;
	}

	return final_offset_y;
}

CardinalDirection Labyrinth::getAbsDirectionFromRelativeDirection(RelativeDirection direction) const {
	int cardinal_offset = 0;

	switch (direction) {
	case RIGHT:
		cardinal_offset = 1;
		break;
	case BACK:
		cardinal_offset = 2;
		break;
	case LEFT:
		cardinal_offset = 3;
		break;
	}

	return static_cast<CardinalDirection>((static_cast<int>(pov_direction) + cardinal_offset) % 4);
}

GroundTypeId Labyrinth::getGroundRel(int x_offset, int y_offset) const
{
	return getGroundAbs(getAbsXFromPovX(x_offset, y_offset), getAbsYFromPovY(x_offset, y_offset));
}

WallTypeId Labyrinth::getWallRel(int x_offset, int y_offset, RelativeDirection direction) const
{
	CardinalDirection abs_direction = getAbsDirectionFromRelativeDirection(direction);
	int abs_x_offset = getAbsXFromPovX(x_offset, y_offset);
	int abs_y_offset = getAbsYFromPovY(x_offset, y_offset);

	switch (abs_direction) {
	case NORTH:
		return getWallAbs(abs_x_offset, abs_y_offset + 1, HORIZONTAL);
		break;
	case EAST:
		return getWallAbs(abs_x_offset + 1, abs_y_offset, VERTICAL);
		break;
	case SOUTH:
		return getWallAbs(abs_x_offset, abs_y_offset, HORIZONTAL);
		break;
	case WEST:
		return getWallAbs(abs_x_offset, abs_y_offset, VERTICAL);
		break;
	}

	return WallTypeId();
}



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

bool Labyrinth::movePovRel(int x_offset, int y_offset)
{
	int new_pov_x = getAbsXFromPovX(x_offset, y_offset);
	if (new_pov_x < 0 || new_pov_x >= x_size) {
		return false;
	}

	int new_pov_y = getAbsYFromPovY(x_offset, y_offset);
	if (new_pov_y < 0 || new_pov_y >= y_size) {
		return false;
	}

	pov_x = new_pov_x;
	pov_y = new_pov_y;
	return true;
}

bool Labyrinth::advance() {
	if (!getWallRel(0, 0, FRONT)) {
		return movePovRel(0, 1);
	}
	else {
		return false;
	}
}

bool Labyrinth::moveBack() {
	if (!getWallRel(0, 0, BACK)) {
		return movePovRel(0, -1);
	}
	else {
		return false;
	}
}

void Labyrinth::turnPovRel(RelativeDirection direction)
{
	switch (direction) {
	case LEFT:
		pov_direction = (CardinalDirection) (((int) pov_direction + 3) % 4);
		break;
	case BACK:
		pov_direction = (CardinalDirection)(((int)pov_direction + 2) % 4);
		break;
	case RIGHT:
		pov_direction = (CardinalDirection)(((int)pov_direction + 1) % 4);
		break;
	}
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

GroundTypeId Labyrinth::getGroundAbs(int x, int y) const
{
	if (x < 0 || y < 0 || x >= (int) x_size || y >= (int) y_size) {
		return 0;
	}

	return ground[y * x_size + x];
}

std::string Labyrinth::printXLineToString(unsigned int y) const {
	std::string retVal;
	for (int x = 0; x < x_size; ++x) {
		retVal.append("+");
		if (getWallAbs(x, y, HORIZONTAL)) {
			retVal.append("-");
		}
		else {
			retVal.append(" ");
		}
	}
	retVal.append("+\n");

	return retVal;
}

std::string Labyrinth::printGroundTileToString(unsigned int x, unsigned int y) const {
	std::string retVal;
	GroundTypeId groundVal = getGroundAbs(x, y);
	if (pov_x == x && pov_y == y) {
		std::string pov_string(" ");
		switch (pov_direction) {
		case NORTH:
			pov_string = "\x1E";
			break;
		case EAST:
			pov_string = "\x10";
			break;
		case SOUTH:
			pov_string = "\x1F";
			break;
		case WEST:
			pov_string = "\x11";
			break;
		}
		retVal.append(pov_string);
	}
	else if (groundVal) {
		retVal.append(std::to_string(groundVal));
	}
	else {
		retVal.append(" ");
	}

	return retVal;
}

std::string Labyrinth::printYLineToString(unsigned int y) const {
	std::string retVal;
	for (int x = 0; x < x_size + 1; ++x) {
		if (getWallAbs(x, y, VERTICAL)) {
			retVal.append("|");
		}
		else {
			retVal.append(" ");
		}

		retVal.append(printGroundTileToString(x, y));
	}
	retVal.append("\n");
	return retVal;
}

std::string Labyrinth::printToString() const {
	std::string retVal;
	retVal.append(printXLineToString(y_size));
	for (int y = y_size - 1; y >= 0; --y) {
		retVal.append(printYLineToString(y));
		retVal.append(printXLineToString(y));
	}
	return retVal;
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

int Labyrinth::addEntity(Entity entity)
{
	entities.push_back(entity);
	return entities.size();
}

Entity* Labyrinth::getEntity(int index) {
	if (index < entities.size() - 1) {
		return &entities[index];
	} else {
		return nullptr;
	}
}

Entity* Labyrinth::getEntityAbs(int x, int y) {
	for (int i = 0; i < entities.size(); ++i) {
		if (entities[i].getX() == x && entities[i].getY() == y) {
			return &entities[i];
		}
	}
	
	return nullptr;
}

Entity* Labyrinth::getEntityRel(int x, int y) {
	int abs_x_offset = getAbsXFromPovX(x, y);
	int abs_y_offset = getAbsYFromPovY(x, y);

	for (int i = 0; i < entities.size(); ++i) {
		if (entities[i].getX() == abs_x_offset && entities[i].getY() == abs_y_offset) {
			return &entities[i];
		}
	}

	return nullptr;
}
