#pragma once
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>

typedef unsigned int TextureId;

typedef int TileId;
typedef std::vector<TileId> TileVec;

void writeTileVec(const TileVec& tiles, std::ofstream& stream);
TileVec readTileVec(std::ifstream& stream);

struct TextureInfo {
	TextureId id;
	int tile_size_x;
	int tile_size_y;
	std::string texture_filename;
	std::string name;
	std::shared_ptr<sf::Texture> texture;

	sf::IntRect getTextureRect(int index) const;
	bool operator==(const TextureInfo& info) const;
};