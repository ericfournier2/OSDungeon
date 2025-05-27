#pragma once
#include <fstream>
#include <map>
#include "common.h"
#include "texture.h"

typedef unsigned int WallId;

struct WallTexture {
	TextureId texture = 0;
	TileVec tiles = { 0 };

	bool write(std::ofstream& stream) const {
		stream.write(reinterpret_cast<const char*>(&texture), sizeof(TextureId));
		writeTileVec(tiles, stream);

		return !stream.fail();
	}
	bool read(std::ifstream& stream) {
		stream.read(reinterpret_cast<char*>(&texture), sizeof(TextureId));
		tiles = readTileVec(stream);

		return !stream.fail();
	}
};

typedef std::map<int, WallTexture> WallTextureMap;

struct WallInfo {
	WallId id = 0;
	std::string name = "";
	ColorUInt8 color = sf::Color::White;
	WallTexture front;
	WallTexture partial;
	WallTextureMap depth_map;

	bool write(std::ofstream& stream) const;
	bool read(std::ifstream& stream);
};
