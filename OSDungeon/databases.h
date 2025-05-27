#pragma once

#include <map>
#include <fstream>
#include <vector>

#include "common.h"
#include "texture.h"

#include "ground.h"
#include "wall.h"
#include "entity_template.h"
#include "sprite.h"


template <typename TId, typename TInfo>
class TemplateDb {
public:
	using IdType = typename TId;
	using InfoType = typename TInfo;

	bool readFromStream(std::ifstream& stream) {
		db_map.clear();
		auto db_size = db_map.size();
		stream.read(reinterpret_cast<char *>(&db_size), sizeof(db_size));
		for (int i = 0; i < db_size; ++i) {
			TInfo info;
			info.read(stream);
			db_map.emplace(info.id, info);
		}
		return !stream.fail();
	}
	
	bool readFromFile(const std::string& filename) {
		bool success = false;
		std::ifstream stream;
		stream.open(filename, std::ifstream::in | std::ifstream::binary);
		if (!stream.fail()) {
			success = readFromStream(stream);
		}
		return success;
	}

	bool writeToStream(std::ofstream& stream) const {
		auto db_size = db_map.size();
		stream.write(reinterpret_cast<char*>(&db_size), sizeof(db_size));
		for (const auto& [key, value] : db_map) {
			value.write(stream);
		}
		return !stream.fail();
	}

	bool writeToFile(const std::string& filename) const {
		bool success = false;
		std::ofstream stream;
		stream.open(filename, std::ofstream::out | std::ofstream::binary);
		if (!stream.fail()) {
			success = writeToStream(stream);
		} 
		return success;
	}
	
	TInfo getElement(TId id) const {
		if (db_map.contains(id)) {
			return db_map.at(id);
		}
		else {
			return TInfo();
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
	
	bool updateElement(TInfo info) {
		if (db_map.contains(info.id)) {
			db_map[info.id] = info;
			return false;
		} else {
			addElement(info);
			return true;
		}
	}

	bool removeElement(TId id) {
		if (db_map.contains(id)) {
			db_map.erase(id);
			return true;
		} else {
			return false;
		}
	}
	
	std::vector<TId> getIds() const {
		auto retVal = std::vector<TId>();
		for (auto const& imap : db_map) {
			retVal.push_back(imap.first);
		}
		return retVal;
	}
private:
	std::map<TId, TInfo> db_map;
};

typedef TemplateDb<WallId, WallInfo> WallDb;
typedef TemplateDb<GroundId, GroundInfo> GroundDb;
typedef TemplateDb<EntityTemplateId, EntityTemplateInfo> EntityTemplateDb;
typedef TemplateDb<SpriteId, SpriteInfo> SpriteDb;

class TextureDb {
public:
	TextureDb();
	TextureId loadNewTexture(TextureId id, const std::string& filename, const std::string& name, int tile_size_x = 0, int tile_size_y = 0);
	TextureInfo getTexture(TextureId id) const;
	std::vector<TextureId> getIds() const;
	bool writeToFile(const std::string& filename) const;
	bool readFromFile(const std::string& filename);
	bool writeToStream(std::ofstream& stream) const;
	bool readFromStream(std::ifstream& stream);
private:
	std::map<TextureId, TextureInfo> texture_map;
	std::shared_ptr<sf::Texture> empty_texture = std::make_shared<sf::Texture>();
	TextureInfo empty_info;
};

struct Databases {
	GroundDb& gdb;
	WallDb& wdb;
	TextureDb& tdb;
	EntityTemplateDb& edb;
	SpriteDb& sdb;
};