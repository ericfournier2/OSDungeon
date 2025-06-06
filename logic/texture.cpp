#include "texture.h"

void writeTileVec(const TileVec& tiles, std::ofstream& stream) {
	auto vec_size = tiles.size();
	stream.write(reinterpret_cast<const char*>(&vec_size), sizeof(vec_size));
	for (const auto& tile : tiles) {
		stream.write(reinterpret_cast<const char*>(&tile), sizeof(tile));
	}
}

TileVec readTileVec(std::ifstream& stream) {
	TileVec retval;
	auto vec_size = retval.size();

	stream.read(reinterpret_cast<char*>(&vec_size), sizeof(vec_size));
	if (vec_size > 50) {
		throw;
	}
	for (int c = 0; c < vec_size; ++c) {
		TileId tile_id;
		stream.read(reinterpret_cast<char*>(&tile_id), sizeof(tile_id));
		retval.push_back(tile_id);
	}

	return retval;
}

sf::IntRect TextureInfo::getTextureRect(int index) const {
	auto tex_size = texture->getSize();
	if (tex_size.x == 0 || tex_size.y == 0) {
		return sf::IntRect({ 0, 0 }, { 1, 1 });
	}

	int tile_per_row = 1;
	int tile_per_col = 1;
	if (tile_size_x != 0) {
		tile_per_row = tex_size.x / tile_size_x;
	}

	if (tile_size_y != 0) {
		tile_per_col = tex_size.y / tile_size_y;
	}

	int row = index / tile_per_row;
	int col = index % tile_per_row;

	int pos_x = col * tile_size_x;
	int pos_y = row * tile_size_y;

	if (tile_size_x == 0 || tile_size_y == 0) {
		return sf::IntRect({ pos_x, pos_y }, { (int)tex_size.x, (int)tex_size.y });
	}

	return sf::IntRect({ pos_x, pos_y }, { tile_size_x, tile_size_y });
}

bool TextureInfo::operator==(const TextureInfo& info) const {
	return id == info.id &&
		tile_size_x == info.tile_size_x &&
		tile_size_y == info.tile_size_y &&
		texture_filename == info.texture_filename &&
		name == info.name;
}