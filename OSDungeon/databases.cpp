#include <memory>
#include "databases.h"


TextureDb::TextureDb() {
	loadNewTexture(1, "Elora.png");
	loadNewTexture(2, "Coralie.png");
}

TextureId TextureDb::loadNewTexture(TextureId id, const std::string& filename) {
	auto ptr = std::make_shared<sf::Texture>(filename);
	TextureInfo info = TextureInfo({ id, filename, ptr });
	texture_map.emplace(id, info);

	return id;
}

std::optional<TextureInfo> TextureDb::getTexture(TextureId id) {
	if (texture_map.contains(id)) {
		return texture_map[id];
	} else {
		return std::nullopt;
	}
}