#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>
#include <fstream>
#include <memory>
#include <vector>

typedef unsigned int WallTypeId;
typedef unsigned int GroundTypeId;
typedef unsigned int TextureId;
typedef int EntityTemplateId;

struct WallInfo {
	WallTypeId id = 0;
	sf::Color color = sf::Color::White;
	TextureId texture = 0;
};

struct GroundInfo {
	GroundTypeId id = 0;
	sf::Color ground_color = sf::Color::White;
	sf::Color ceiling_color = sf::Color::White;
	TextureId texture = 0;
};

enum EntityBehaviourType {
	DOODAD,
	PICKABLE,
	WANDERING,
	LETHAL
};

struct EntityTemplateInfo {
	EntityTemplateId id;
	EntityBehaviourType behaviour;
	float x_size = 0.0f;
	float y_size = 0.0f;
	float x_offset = 0.0f;
	float y_offset = 0.0f;
	TextureId texture = 0;
};

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
			stream.read(reinterpret_cast<char*>(&info), sizeof(TInfo));
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
			stream.write(reinterpret_cast<const char*>(&value), sizeof(value));
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

typedef TemplateDb<WallTypeId, WallInfo> WallDb;
typedef TemplateDb<GroundTypeId, GroundInfo> GroundDb;
typedef TemplateDb<EntityTemplateId, EntityTemplateInfo> EntityTemplateDb;

struct TextureInfo {
	TextureId id;
	std::string texture_filename;
	std::shared_ptr<sf::Texture> texture;
};

class TextureDb {
public:
	TextureDb();
	TextureId loadNewTexture(TextureId id, const std::string& filename);
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
};