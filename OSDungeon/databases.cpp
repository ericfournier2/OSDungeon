#include <memory>
#include "databases.h"


TextureDb::TextureDb() {
	empty_info.id = 0;
	empty_info.texture_filename = "";
	empty_info.texture = empty_texture;
}

TextureId TextureDb::loadNewTexture(TextureId id, const std::string& filename) {
	auto ptr = std::make_shared<sf::Texture>(filename);
	TextureInfo info = TextureInfo({ id, filename, ptr });
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
		std::string filename;
		std::getline(stream, filename, '\0');
		loadNewTexture(id, filename);
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
		//stream.write(reinterpret_cast<const char*>(value.texture_filename.c_str()), value.texture_filename.size() + 1);
		stream << value.texture_filename << '\0';
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