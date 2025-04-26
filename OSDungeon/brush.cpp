#include "brush.h"

Brush::Brush(const EntityTemplateDb& template_db_init) 
	: template_db(template_db_init)
{
}

void Brush::apply(Labyrinth& labyrinth, float x, float y) const {
	BrushPreview brush_preview = preview(labyrinth, x, y);
	applyPreview(labyrinth, brush_preview);
}

BrushPreview Brush::preview(const Labyrinth& labyrinth, float x, float y) const {
	switch (brush_type) {
	case BrushShape::BRUSH_SHAPE_POINT:
		return pointPreview(labyrinth, x, y);
		break;
	case BrushShape::BRUSH_SHAPE_WALL:
		return wallPreview(labyrinth, x, y);
		break;
	default:
		assert(false);
	}

	// Should never be reached, but return an empty preview anyway.
	return BrushPreview();
}

void Brush::applyPreview(Labyrinth& labyrinth, const BrushPreview& brush_preview) const {
	for (auto const& [key, val] : brush_preview.grounds) {
		labyrinth.setGround(key.x, key.y, val);
	}

	for (auto const& [key, val] : brush_preview.walls) {
		if (val == 0) {
			labyrinth.removeWall(key.x, key.y, key.o);
		} else {
			labyrinth.addWall(key.x, key.y, key.o, val);
		}
	}

	for (auto const& [key, val] : brush_preview.entities) {
		if (val == 0) {
			ShallowEntityVec entities = labyrinth.getEntityManager().getEntityAbs(key.x, key.y);
			for (auto const& entity : entities) {
				labyrinth.getEntityManager().removeEntity(entity.id);
			}
		} else {
			//labyrinth.addWall(key.x, key.y, key.o, val);
			labyrinth.getEntityManager().addEntity(val, key.x, key.y, CardinalDirection::NORTH);
		}
	}
}

BrushPreview Brush::pointPreview(const Labyrinth& labyrinth, float x, float y) const {
	// Snap coordinates to integer coordinates.
	Coord base_coord = { static_cast<int>(x), static_cast<int>(y) };

	BrushPreview retval;
	if (brush_fill == BrushFill::BRUSH_FILL_GROUND || brush_fill == BrushFill::BRUSH_FILL_AREA) {
		if (brush_action == BrushAction::BRUSH_ACTION_DRAW) {
			retval.grounds[base_coord] = ground_id;
		} else {
			retval.grounds[base_coord] = 0;
		}
	}

	if (brush_fill == BrushFill::BRUSH_FILL_ENTITY) {
		if (brush_action == BrushAction::BRUSH_ACTION_DRAW) {
			retval.entities[base_coord] = entity_id;
		} else {
			retval.entities[base_coord] = 0;
		}
	}

	if (brush_fill == BrushFill::BRUSH_FILL_WALL || brush_fill == BrushFill::BRUSH_FILL_AREA) {
		enclosingWall(retval, labyrinth, base_coord, CardinalDirection::NORTH);
		enclosingWall(retval, labyrinth, base_coord, CardinalDirection::EAST);
		enclosingWall(retval, labyrinth, base_coord, CardinalDirection::WEST);
		enclosingWall(retval, labyrinth, base_coord, CardinalDirection::SOUTH);
	}

	return retval;
}

void Brush::enclosingWall(BrushPreview& preview, const Labyrinth& labyrinth, Coord coord, CardinalDirection d) const {
	int ground_x_offset = 0;
	int ground_y_offset = 0;
	int wall_x_offset = 0;
	int wall_y_offset = 0;
	WallOrientation orient = WallOrientation::HORIZONTAL;

	switch (d) {
	case CardinalDirection::EAST:
		ground_x_offset = 1;
		wall_x_offset = 1;
		orient = WallOrientation::VERTICAL;
		break;
	case CardinalDirection::NORTH:
		ground_y_offset = 1;
		wall_y_offset = 1;
		break;
	case CardinalDirection::WEST:
		ground_x_offset = -1;
		orient = WallOrientation::VERTICAL;
		break;
	case CardinalDirection::SOUTH:
		ground_y_offset = -1;
		break;
	}
	
	GroundId brush_id = brush_action == BRUSH_ACTION_DRAW ? wall_id : 0;
	GroundId other_ground = labyrinth.getGroundAbs(coord.x + ground_x_offset, coord.y + ground_y_offset);
	WallCoord wall_coord = { coord.x + wall_x_offset, coord.y + wall_y_offset, orient };
	if ((other_ground == 0 && brush_id == 0) || (other_ground != 0 && brush_id != 0)) {
		preview.walls[wall_coord] = 0;
	} else {
		preview.walls[wall_coord] = brush_id;
	}
}

bool Brush::isMin4(float x, float o1, float o2, float o3) const {
	return x < o1 && x < 02 && x < o3 && o1 > wall_dead_zone && o2 > wall_dead_zone && o3 > wall_dead_zone;
}

BrushPreview Brush::wallPreview(const Labyrinth& labyrinth, float x, float y) const {
	// Find closest wall.
	float x_left_dist = x - static_cast<int>(x);
	float y_bottom_dist = y - static_cast<int>(y);
	float x_right_dist = 1 - x_left_dist;
	float y_top_dist = 1 - y_bottom_dist;

	BrushPreview retval;
	int int_x = static_cast<int>(x);
	int int_y = static_cast<int>(y);
	if (isMin4(x_left_dist, y_bottom_dist, x_right_dist, y_top_dist)) {
		retval.walls[{int_x, int_y, WallOrientation::VERTICAL}] = wall_id;
	} else if (isMin4(y_bottom_dist, x_left_dist, x_right_dist, y_top_dist)) {
		retval.walls[{int_x, int_y, WallOrientation::HORIZONTAL}] = wall_id;
	} else if (isMin4(x_right_dist, y_bottom_dist, x_left_dist, y_top_dist)) {
		retval.walls[{int_x + 1, int_y, WallOrientation::VERTICAL}] = wall_id;
	} else if (isMin4(y_top_dist, y_bottom_dist, x_right_dist, x_left_dist)) {
		retval.walls[{int_x, int_y + 1, WallOrientation::HORIZONTAL}] = wall_id;
	}

	return retval;
}