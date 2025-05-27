#include "sprite.h"

bool SpriteInfo::write(std::ofstream& stream) const {
	stream.write(reinterpret_cast<const char*>(&id), sizeof(id));
	stream << name << '\0';
	stream.write(reinterpret_cast<const char*>(&texture), sizeof(texture));

	writeTileVec(front, stream);
	writeTileVec(back, stream);
	writeTileVec(left, stream);
	writeTileVec(right, stream);

	return !stream.fail();
}

bool SpriteInfo::read(std::ifstream& stream) {
	stream.read(reinterpret_cast<char*>(&id), sizeof(SpriteId));
	std::getline(stream, name, '\0');
	stream.read(reinterpret_cast<char*>(&texture), sizeof(TextureId));

	front = readTileVec(stream);
	back = readTileVec(stream);
	left = readTileVec(stream);
	right = readTileVec(stream);
	return !stream.fail();
}

TileVec SpriteInfo::getTileVec(RelativeDirection d) const {
	switch (d) {
	case RelativeDirection::FRONT:
		return front;
		break;
	case RelativeDirection::BACK:
		return back;
		break;
	case RelativeDirection::LEFT:
		return left;
		break;
	case RelativeDirection::RIGHT:
		return right;
		break;
	}

	return front;
}