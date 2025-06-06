#include "ground.h"

bool GroundInfo::write(std::ofstream& stream) const {
	stream.write(reinterpret_cast<const char*>(&id), sizeof(GroundId));
	stream << name << '\0';
	stream.write(reinterpret_cast<const char*>(&ground_color), sizeof(ground_color));
	stream.write(reinterpret_cast<const char*>(&ceiling_color), sizeof(ceiling_color));
	stream.write(reinterpret_cast<const char*>(&texture), sizeof(texture));
	stream.write(reinterpret_cast<const char*>(&draw_ceiling), sizeof(draw_ceiling));

	return !stream.fail();
}

bool GroundInfo::read(std::ifstream& stream) {
	stream.read(reinterpret_cast<char*>(&id), sizeof(GroundId));
	std::getline(stream, name, '\0');
	stream.read(reinterpret_cast<char*>(&ground_color), sizeof(ground_color));
	stream.read(reinterpret_cast<char*>(&ceiling_color), sizeof(ceiling_color));
	stream.read(reinterpret_cast<char*>(&texture), sizeof(texture));
	stream.read(reinterpret_cast<char*>(&draw_ceiling), sizeof(draw_ceiling));
	return !stream.fail();
}

bool GroundInfo::operator==(const GroundInfo& info) const {
	return id == info.id &&
		name == info.name &&
		ground_color == info.ground_color &&
		ceiling_color == info.ceiling_color &&
		texture == info.texture &&
		draw_ceiling == info.draw_ceiling;
}