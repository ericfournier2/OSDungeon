#include "view_utils.h"

sf::Sprite getAnimationSprite(const SpriteInfo& sprite_info, const TextureInfo& tex_info, RelativeDirection d, const sf::Clock& animation_clock) {
	
	sf::Sprite sprite(*tex_info.texture);
	TileVec tiles = sprite_info.getTileVec(d);

	sf::Time animation_time = animation_clock.getElapsedTime();
	int millisecond = animation_time.asMilliseconds() % 1000;
	auto tile = (millisecond / (1000 / tiles.size())) % tiles.size();
	sprite.setTextureRect(tex_info.getTextureRect(abs(tiles[tile])));

	float mirror_scale = 1.0f;
	if (tiles[tile] < 0) {
		mirror_scale = -1.0f;
	}

	//sprite.setPosition({ (float)-size_x, 0.0f });
	sprite.setScale({ mirror_scale, 1.0f });

	return sprite;
}

sf::Sprite getAnimationSprite(const SpriteInfo& sprite_info, RelativeDirection d, const sf::Clock& animation_clock, const TextureDb& tdb) {
	TextureInfo tex_info = tdb.getTexture(sprite_info.texture);
	return getAnimationSprite(sprite_info, tex_info, d, animation_clock);
}

sf::Sprite getAnimationSprite(SpriteId id, RelativeDirection d, const sf::Clock& animation_clock, const TextureDb& tdb, const SpriteDb& sdb) {
	SpriteInfo sprite_info = sdb.getElement(id);
	return getAnimationSprite(sprite_info, d, animation_clock, tdb);
}