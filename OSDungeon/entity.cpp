#include "entity.h"

Entity::Entity(ShallowEntity& info_init, const EntityTemplateDb& template_db_init)
	: info(info_init), template_db(template_db_init)
{

}

bool Entity::moveCardinal(const Labyrinth& labyrinth, CardinalDirection d) {
	if (labyrinth.canMove(getX(), getY(), d)) {
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
		state.gameOver("The bunny ate\nyou. =(");
	}

	return false;
}

