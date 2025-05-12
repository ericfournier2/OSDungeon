#include "labyrinth_background.h"
#include "view_utils.h"

void BackgroundView::render(sf::RenderTarget& rt, const LabyrinthBackground& background)
{
	TextureInfo tex_info = db.tdb.getTexture(background.texture);

	sf::RectangleShape backdrop({ (float)tex_info.texture->getSize().x, (float)tex_info.texture->getSize().y });
	backdrop.setTexture(tex_info.texture.get());
	rt.draw(backdrop);

	for (int z = 0; z < 16; ++z) {
		for (auto const& entity : background.entities) {
			if (entity.z_order == z) {
				sf::Sprite sprite = getAnimationSprite(entity.sprite_id, RelativeDirection::FRONT, animation_clock, db.tdb, db.sdb);
				sprite.setPosition(entity.position);
				sprite.scale({ entity.scale_factor, entity.scale_factor });
				rt.draw(sprite);
			}
		}
	}
}
