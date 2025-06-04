#pragma once
#include <fstream>
#include "common.h"
#include "texture.h"

typedef unsigned int GroundId;

struct GroundInfo {
	GroundId id = 0;
	std::string name = "";
	ColorUInt8 ground_color = sf::Color::White;
	ColorUInt8 ceiling_color = sf::Color::White;
	TextureId texture = 0;
	bool draw_ceiling = false;

	bool write(std::ofstream& stream) const;
	bool read(std::ifstream& stream);
};

