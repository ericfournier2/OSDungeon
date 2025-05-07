#include "entity.h"

Entity::Entity(const EntityState& info_init, const EntityTemplateDb& template_db_init, const SpriteDb& sprite_db_init)
	: info(info_init), template_db(template_db_init), sprite_db(sprite_db_init)
{

}

bool Entity::moveCardinal(const Labyrinth& labyrinth, CardinalDirection d) {
	if (labyrinth.canMove(getX(), getY(), d)) {
		info.direction = d;
		switch (d) {
		case NORTH:
			info.y += 1;
			break;
		case SOUTH:
			info.y -= 1;
			break;
		case EAST:
			info.x += 1;
			break;
		case WEST:
			info.x -= 1;
			break;
		}

		return true;
	}

	return false;
}

void Entity::move(const Labyrinth& labyrinth, GameState& state)
{
	if (getMovementType() == MovementType::WANDER) {
		// Pick a random direction
		CardinalDirection d = static_cast<CardinalDirection>(static_cast<int>(rand() % 4));
		moveCardinal(labyrinth, d);
	} else if (getMovementType() == MovementType::FOLLOW) {
		if (labyrinth.hasLOS(state.getPlayerPos().x, state.getPlayerPos().y, info.x, info.y)) {
			std::optional<Path> player_path = labyrinth.findPath(info.x, info.y, state.getPlayerPos().x, state.getPlayerPos().y);
			if (player_path && player_path.value().size() > 0) {
				moveCardinal(labyrinth, player_path.value()[0]);
			}
		}
	}
}

bool Entity::collide(const Labyrinth& labyrinth, GameState& state)
{
	if (getCollisionType() == CollisionType::PICKABLE) {
		state.showDialog("You got the\ntreasure!");
		return true;
	} else if (getCollisionType() == CollisionType::LETHAL) {
		state.gameOver("WWWWWWWWWWWWWWWWWWWWWW Let's write a really long sentence. The kind of sentence that will NEED line wrap. The kind of sentence that will make an epic dialog box. The kind of sentence that never ends. Ooops, that's actually a paragraph, not a single sentence. Shucks. Whatever will I do? The bunny ate you. =(");
	}

	return false;
}

