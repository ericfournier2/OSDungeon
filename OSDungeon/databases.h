#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>
#include <fstream>
#include <memory>

typedef unsigned int WallTypeId;
typedef unsigned int GroundTypeId;
typedef unsigned int TextureId;

struct WallInfo {
	WallTypeId id;
	sf::Color color;
	TextureId texture;
};

struct GroundInfo {
	GroundTypeId id;
	sf::Color color;
	TextureId texture;
};

template <typename TId, typename TInfo>
class TemplateDb {
public:
	bool readFromStream(std::ifstream& stream) {
		db_map.clear();
		auto db_size = db_map.size();
		stream.read(reinterpret_cast<char *>(&db_size), sizeof(db_size));
		for (int i = 0; i < db_size; ++i) {
			TInfo info;
			stream.read(reinterpret_cast<char*>(&info), sizeof(TInfo));
			db_map.emplace(info.id, info);
		}
		return !stream.fail();
	}
	virtual bool writeToStream(std::ofstream& stream) {
		auto db_size = db_map.size();
		stream.write(reinterpret_cast<char*>(&db_size), sizeof(db_size));
		for (const auto& [key, value] : db_map) {
			stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
		return !stream.fail();
	}
	std::optional<TInfo> getElement(TId id) {
		if (db_map.contains(id)) {
			return db_map[id];
		}
		else {
			return std::nullopt;
		}
	}
	TId addElement(TInfo info) {
		// Find a free ID
		TId i = 1;
		while (db_map.contains(i)) {
			++i;
		}
		info.id = i;
		db_map.emplace(i, info);
		return i;
	}
	bool removeElement(TId id) {
		if (db_map.contains(id)) {
			db_map.erase(id);
			return true;
		} else {
			return false;
		}
	}
private:
	std::map<TId, TInfo> db_map;
};

typedef TemplateDb<WallTypeId, WallInfo> WallDb;
typedef TemplateDb<GroundTypeId, GroundInfo> GroundDb;

struct TextureInfo {
	TextureId id;
	std::string texture_filename;
	std::shared_ptr<sf::Texture> texture;
};

class TextureDb {
public:
	TextureDb();
	TextureId loadNewTexture(TextureId id, const std::string& filename);
	std::optional<TextureInfo> getTexture(TextureId id);
private:
	std::map<TextureId, TextureInfo> texture_map;
};