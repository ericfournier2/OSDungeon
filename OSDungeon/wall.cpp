#include "wall.h"

bool WallInfo::write(std::ofstream& stream) const {
	stream.write(reinterpret_cast<const char*>(&id), sizeof(id));
	stream << name << '\0';
	stream.write(reinterpret_cast<const char*>(&color), sizeof(color));
	front.write(stream);
	partial.write(stream);
	auto size = depth_map.size();
	stream.write(reinterpret_cast<const char*>(&size), sizeof(size));
	for (auto const& [key, val] : depth_map) {
		stream.write(reinterpret_cast<const char*>(&key), sizeof(key));
		val.write(stream);
	}

	return !stream.fail();
}
bool WallInfo::read(std::ifstream& stream) {
	stream.read(reinterpret_cast<char*>(&id), sizeof(id));
	std::getline(stream, name, '\0');
	stream.read(reinterpret_cast<char*>(&color), sizeof(color));
	front.read(stream);
	partial.read(stream);
	auto size = depth_map.size();
	stream.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (size > 1000 || stream.fail()) {
		// Sanity check so a malformed input file won't hang the application.
		return false;
	}
	for (int c = 0; c < size; ++c) {
		int key = 0;
		stream.read(reinterpret_cast<char*>(&key), sizeof(key));
		WallTexture val;
		val.read(stream);
		depth_map.emplace(key, val);
	}

	return !stream.fail();
}