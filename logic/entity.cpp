#include "entity.h"
#include "labyrinth.h"

Entity::Entity() {
}

//Entity::Entity(const EntityState& info_init)
//	: entity_state(info_init)
//{
//}

Entity::Entity(const EntityState& info_init, const Databases& db)
	: entity_state(info_init)
{
	refresh(db);
}

void Entity::refresh(const Databases& db) {
	template_info = db.edb.getElement(entity_state.template_id);
	sprite_info = db.sdb.getElement(template_info.sprite_id);
	texture = db.tdb.getTexture(sprite_info.texture);
}

void Entity::update(Databases& db) const {
	db.edb.updateElement(template_info);
	db.sdb.updateElement(sprite_info);
}

//sf::Sprite Entity::getSprite(RelativeDirection d) const {
//	return getAnimationSprite(sprite_info, texture, d, animation_clock);
//}

bool Entity::moveCardinal(const Labyrinth& labyrinth, CardinalDirection d) {
	if (labyrinth.canMove(getX(), getY(), d)) {
		entity_state.direction = d;
		switch (d) {
		case NORTH:
			entity_state.y += 1;
			break;
		case SOUTH:
			entity_state.y -= 1;
			break;
		case EAST:
			entity_state.x += 1;
			break;
		case WEST:
			entity_state.x -= 1;
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
		if (labyrinth.hasLOS(state.getPlayerPos().x, state.getPlayerPos().y, entity_state.x, entity_state.y)) {
			std::optional<Path> player_path = labyrinth.findPath(entity_state.x, entity_state.y, state.getPlayerPos().x, state.getPlayerPos().y);
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

EntityId EntityManager::findFreeId() const {
	// Get first free ID
	EntityId id = 1;
	while (entities.contains(id)) {
		++id;
	}

	return id;
}

EntityId EntityManager::addEntity(const EntityState& entity) {
	if (entities.contains(entity.id)) {
		EntityId new_id = findFreeId();
		EntityState new_info = entity;
		new_info.id = new_id;
		if (db) {
			entities.emplace(new_id, std::make_shared<Entity>(new_info, *db));
		} else {
			entities.emplace(new_id, std::make_shared<Entity>(new_info));
		}
		return new_id;
	} else {
		if (db) {
			entities.emplace(entity.id, std::make_shared<Entity>(entity, *db));
		} else {
			entities.emplace(entity.id, std::make_shared<Entity>(entity));
		}
		return entity.id;
	}
}

EntityId EntityManager::addEntity(EntityTemplateId template_id, int x, int y, CardinalDirection d) {
	EntityId id = findFreeId();
	EntityState state(id, template_id, x, y, d);
	if (db) {
		entities.emplace(id, std::make_shared<Entity>(state, *db));
	} else {
		entities.emplace(id, std::make_shared<Entity>(state));
	}
	return id;
}

bool EntityManager::updateEntity(const Entity& entity) {
	if (entities.contains(entity.getId())) {
		entities[entity.getId()] = std::make_shared<Entity>(entity);
		return true;
	}

	return false;
}

void EntityManager::removeEntity(EntityId id) {
	entities.erase(id);
}

std::shared_ptr<Entity> EntityManager::getEntity(EntityId id) const {
	if (entities.contains(id)) {
		return entities.at(id);
	} else {
		return nullptr;
	}
}

EntityVec EntityManager::getEntityAbs(int x, int y) const {
	EntityVec retval;
	for (auto const& [key, val] : entities) {
		if (val->getState().x == x && val->getState().y == y) {
			retval.push_back(val);
		}
	}

	return retval;
}

bool EntityManager::writeToStream(std::ofstream& stream) const {
	auto db_size = entities.size();
	stream.write(reinterpret_cast<char*>(&db_size), sizeof(db_size));
	for (const auto& [key, value] : entities) {
		EntityState state = value->getState();
		stream.write(reinterpret_cast<const char*>(&state), sizeof(state));
	}
	return !stream.fail();
}

bool EntityManager::readFromStream(std::ifstream& stream) {
	entities.clear();

	auto db_size = entities.size();
	stream.read(reinterpret_cast<char*>(&db_size), sizeof(db_size));
	for (int i = 0; i < db_size; ++i) {
		EntityState info;
		stream.read(reinterpret_cast<char*>(&info), sizeof(EntityState));
		if (db) {
			entities.emplace(info.id, std::make_shared<Entity>(info, *db));
		} else {
			entities.emplace(info.id, std::make_shared<Entity>(info));
		}
	}
	return !stream.fail();
}

