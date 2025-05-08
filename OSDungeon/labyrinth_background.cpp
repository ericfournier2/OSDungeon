#include "labyrinth_background.h"

void BackgroundView::render(sf::RenderTarget& rt, const LabyrinthBackground& background)
{
	TextureInfo tex_info = db.tdb.getTexture(background.texture);

	sf::RectangleShape backdrop({ (float)tex_info.texture->getSize().x, (float)tex_info.texture->getSize().y });
	backdrop.setTexture(tex_info.texture.get());
	rt.draw(backdrop);

	for (int z = 0; z < 16; ++z) {
		for (auto const& entity : background.entities) {
			if (entity.z_order == z) {
				SpriteInfo sprite_info = db.sdb.getElement(entity.sprite_id);
				TextureInfo sprite_tex_info = db.tdb.getTexture(sprite_info.texture);
				sf::Sprite sprite(*sprite_tex_info.texture);
				TileVec tiles = sprite_info.front;

				sf::Time animation_time = animation_clock.getElapsedTime();
				int millisecond = animation_time.asMilliseconds() % 1000;
				auto tile = (millisecond / (1000 / tiles.size())) % tiles.size();
				sprite.setTextureRect(sprite_tex_info.getTextureRect(abs(tiles[tile])));

				sprite.setPosition(entity.position);
				sprite.setScale({ entity.scale_factor, entity.scale_factor });
				rt.draw(sprite);
			}
		}
	}
}
