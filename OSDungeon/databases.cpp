#include <memory>
#include "databases.h"


TextureDb::TextureDb() {
	empty_info.id = 0;
	empty_info.texture_filename = "";
	empty_info.texture = empty_texture;

	loadNewTexture(1, "Elora.png");
	loadNewTexture(2, "Coralie.png");
	loadNewTexture(3, "BigBricksTexture20.png"); 
	loadNewTexture(4, "Ground2.png");
}

TextureId TextureDb::loadNewTexture(TextureId id, const std::string& filename) {
	auto ptr = std::make_shared<sf::Texture>(filename);
	TextureInfo info = TextureInfo({ id, filename, ptr });
	texture_map.emplace(id, info);

	return id;
}

TextureInfo TextureDb::getTexture(TextureId id) {
	if (texture_map.contains(id)) {
		return texture_map[id];
	} else {
		return empty_info;
	}
}