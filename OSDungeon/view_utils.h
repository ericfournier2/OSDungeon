#pragma once
#include <SFML/Graphics.hpp>
#include "databases.h"

sf::Sprite getAnimationSprite(const SpriteInfo& sprite_info, RelativeDirection d, const sf::Clock& animation_clock, const TextureDb& tdb);
sf::Sprite getAnimationSprite(SpriteId id, RelativeDirection d, const sf::Clock& animation_clock, const TextureDb& tdb, const SpriteDb& sdb);