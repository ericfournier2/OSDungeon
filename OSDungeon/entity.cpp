#include "entity.h"

Entity::Entity(ShallowEntity& info_init, const EntityTemplateDb& template_db_init)
	: info(info_init), template_db(template_db_init)
{

}

void Entity::move(const Labyrinth& labyrinth, GameState& state)
{
	if (getMovementType() == MovementType::WANDER) {
		// Pick a random direction
		CardinalDirection d = static_cast<CardinalDirection>(static_cast<int>(rand() % 4));
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

			//entities.updateEntity(new_entity);
		}
	} else if (getMovementType() == MovementType::FOLLOW) {
		
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

