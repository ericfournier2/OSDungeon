#include <memory>
#include "databases.h"

sf::IntRect TextureInfo::getTextureRect(int index) const {
	auto tex_size = texture->getSize();
	int tile_per_row = tex_size.x / tile_size_x;
	int tile_per_col = tex_size.y / tile_size_y;
	
	int row = index / tile_per_row;
	int col = index % tile_per_row;

	int pos_x = col * tile_size_x;
	int pos_y = row * tile_size_y;

	return sf::IntRect({ pos_x, pos_y }, { tile_size_x, tile_size_y });
}

TextureDb::TextureDb() {
	empty_info.id = 0;
	empty_info.texture_filename = "";
	empty_info.texture = empty_texture;
}

TextureId TextureDb::loadNewTexture(TextureId id, const std::string& filename, const std::string& name, int tile_size_x, int tile_size_y) {
	std::string full_filename("assets\\textures\\");
	full_filename.append(filename);
	auto ptr = std::make_shared<sf::Texture>(filename);
	if (tile_size_x == 0) {
		tile_size_x = ptr->getSize().x;
		tile_size_y = ptr->getSize().y;
	}
	TextureInfo info = TextureInfo({ id, tile_size_x, tile_size_y, filename, name, ptr });
	texture_map.emplace(id, info);

	return id;
}

std::vector<TextureId> TextureDb::getIds() const {
	auto retVal = std::vector<TextureId>();
	for (auto const& imap : texture_map) {
		retVal.push_back(imap.first);
	}
	return retVal;
}

TextureInfo TextureDb::getTexture(TextureId id) const {
	if (texture_map.contains(id)) {
		return texture_map.at(id);
	} else {
		return empty_info;
	}
}

bool TextureDb::readFromStream(std::ifstream& stream) {
	texture_map.clear();
	auto db_size = texture_map.size();
	stream.read(reinterpret_cast<char*>(&db_size), sizeof(db_size));
	for (int i = 0; i < db_size; ++i) {
		TextureId id;
		stream.read(reinterpret_cast<char*>(&id), sizeof(id));
		int tile_size_x = 0;
		stream.read(reinterpret_cast<char*>(&tile_size_x), sizeof(tile_size_x));
		int tile_size_y = 0;
		stream.read(reinterpret_cast<char*>(&tile_size_y), sizeof(tile_size_y));
		std::string filename;
		std::getline(stream, filename, '\0');
		std::string name;
		std::getline(stream, name, '\0');
		loadNewTexture(id, filename, name, tile_size_x, tile_size_y);
	}
	return !stream.fail();
}

bool TextureDb::readFromFile(const std::string& filename) {
	bool success = false;
	std::ifstream stream;
	stream.open(filename, std::ifstream::in | std::ifstream::binary);
	if (!stream.fail()) {
		success = readFromStream(stream);
	}
	return success;
}

bool TextureDb::writeToStream(std::ofstream& stream) const {
	auto db_size = texture_map.size();
	stream.write(reinterpret_cast<char*>(&db_size), sizeof(db_size));
	for (const auto& [key, value] : texture_map) {
		stream.write(reinterpret_cast<const char*>(&key), sizeof(key));
		stream.write(reinterpret_cast<const char*>(&value.tile_size_x), sizeof(value.tile_size_x));
		stream.write(reinterpret_cast<const char*>(&value.tile_size_y), sizeof(value.tile_size_y));
		//stream.write(reinterpret_cast<const char*>(value.texture_filename.c_str()), value.texture_filename.size() + 1);
		stream << value.texture_filename << '\0';
		stream << value.name << '\0';
	}
	return !stream.fail();
}

bool TextureDb::writeToFile(const std::string& filename) const {
	bool success = false;
	std::ofstream stream;
	stream.open(filename, std::ofstream::out | std::ofstream::binary);
	if (!stream.fail()) {
		success = writeToStream(stream);
	}
	return success;
}

void writeTileVec(TileVec tiles, std::ofstream& stream) {
	auto vec_size = tiles.size();
	stream.write(reinterpret_cast<const char*>(&vec_size), sizeof(vec_size));
	for (const auto& tile : tiles) {
		stream.write(reinterpret_cast<const char*>(&tile), sizeof(tile));
	}
}

bool SpriteInfo::write(std::ofstream& stream) const {
	stream.write(reinterpret_cast<const char*>(&id), sizeof(SpriteId));
	stream << name << "\0";
	stream.write(reinterpret_cast<const char*>(&texture), sizeof(TextureId));

	writeTileVec(front, stream);
	writeTileVec(back, stream);
	writeTileVec(left, stream);
	writeTileVec(right, stream);

	return !stream.fail();
}

bool EntityTemplateInfo::write(std::ofstream& stream) const {
	stream.write(reinterpret_cast<const char*>(&id), sizeof(EntityTemplateId));
	stream.write(reinterpret_cast<const char*>(&sprite_id), sizeof(SpriteId));
	stream << name << "\0";
	stream.write(reinterpret_cast<const char*>(&movement), sizeof(MovementType));
	stream.write(reinterpret_cast<const char*>(&collision), sizeof(CollisionType));
	stream.write(reinterpret_cast<const char*>(&x_size), sizeof(float));
	stream.write(reinterpret_cast<const char*>(&y_size), sizeof(float));

	return !stream.fail();
}

TileVec readTileVec(std::ifstream& stream) {
	TileVec retval;
	auto vec_size = retval.size();

	stream.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
	for (int c = 0; c < vec_size; ++c) {
		TileId tile_id;
		stream.read(reinterpret_cast<char*>(&tile_id), sizeof(tile_id));
		retval.push_back(tile_id);
	}

	return retval;
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

bool EntityTemplateInfo::read(std::ifstream& stream) {
	stream.read(reinterpret_cast<char*>(&id), sizeof(EntityTemplateId));
	stream.read(reinterpret_cast<char*>(&sprite_id), sizeof(SpriteId));
	std::getline(stream, name, '\0');
	stream.read(reinterpret_cast<char*>(&movement), sizeof(MovementType));
	stream.read(reinterpret_cast<char*>(&collision), sizeof(CollisionType));
	stream.read(reinterpret_cast<char*>(&x_size), sizeof(float));
	stream.read(reinterpret_cast<char*>(&y_size), sizeof(float));

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