#pragma once
#include "common.h"
#include "texture.h"
#include <fstream>

typedef unsigned int SpriteId;

struct SpriteInfo {
	SpriteId id = 0;
	TextureId texture = 0;
	std::string name = "";
	TileVec front = { 0 };
	TileVec back = { 0 };
	TileVec left = { 0 };
	TileVec right = { 0 };

	bool write(std::ofstream& stream) const;
	bool read(std::ifstream& stream);
	TileVec getTileVec(RelativeDirection d) const;
	bool operator==(const SpriteInfo& info) const;
};