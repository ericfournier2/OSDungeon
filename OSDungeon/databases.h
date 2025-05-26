#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <optional>
#include <fstream>
#include <memory>
#include <vector>

#include "common.h"

typedef unsigned int WallId;
typedef unsigned int GroundId;
typedef unsigned int TextureId;
typedef unsigned int SpriteId;
typedef unsigned int EntityTemplateId;

typedef int TileId;
typedef std::vector<TileId> TileVec;

void writeTileVec(const TileVec& tiles, std::ofstream& stream);
TileVec readTileVec(std::ifstream& stream);

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
	sf::Color color = sf::Color::White;
	WallTexture front;
	WallTexture partial;
	WallTextureMap depth_map;

	bool write(std::ofstream& stream) const {
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
	bool read(std::ifstream& stream) {
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
};

struct GroundInfo {
	GroundId id = 0;
	std::string name = "";
	sf::Color ground_color = sf::Color::White;
	sf::Color ceiling_color = sf::Color::White;
	TextureId texture = 0;
	bool draw_ceiling = false;

	bool write(std::ofstream& stream) const {
		stream.write(reinterpret_cast<const char*>(&id), sizeof(GroundId));
		stream << name << '\0';
		stream.write(reinterpret_cast<const char*>(&ground_color), sizeof(ground_color));
		stream.write(reinterpret_cast<const char*>(&ceiling_color), sizeof(ceiling_color));
		stream.write(reinterpret_cast<const char*>(&texture), sizeof(texture));
		stream.write(reinterpret_cast<const char*>(&draw_ceiling), sizeof(draw_ceiling));

		return !stream.fail();
	}
	bool read(std::ifstream& stream) {
		stream.read(reinterpret_cast<char*>(&id), sizeof(GroundId));
		std::getline(stream, name, '\0');
		stream.read(reinterpret_cast<char*>(&ground_color), sizeof(ground_color));
		stream.read(reinterpret_cast<char*>(&ceiling_color), sizeof(ceiling_color));
		stream.read(reinterpret_cast<char*>(&texture), sizeof(texture));
		stream.read(reinterpret_cast<char*>(&draw_ceiling), sizeof(draw_ceiling));
		return !stream.fail();
	}
};

enum class MovementType {
	STATIC,
	WANDER,
	FOLLOW,
	FLEE,
	PATH
};

enum class CollisionType {
	NONE,
	PICKABLE,
	DIALOG,
	LETHAL
};

enum class InteractionType {
	NONE,
	DIALOG
};

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
};

struct EntityTemplateInfo {
	EntityTemplateId id;
	SpriteId sprite_id;
	std::string name;
	MovementType movement;
	CollisionType collision;
	InteractionType interaction;
	float x_size = 0.0f;
	float y_size = 0.0f;
	bool cast_shadow = false;

	bool write(std::ofstream& stream) const;
	bool read(std::ifstream& stream);
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

struct TextureInfo {
	TextureId id;
	int tile_size_x;
	int tile_size_y;
	std::string texture_filename;
	std::string name;
	std::shared_ptr<sf::Texture> texture;

	sf::IntRect getTextureRect(int index) const;
};

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