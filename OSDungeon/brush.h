#pragma once
#include "labyrinth.h"
#include "databases.h"

enum BrushShape {
	BRUSH_SHAPE_WALL,
	BRUSH_SHAPE_POINT,
	BRUSH_SHAPE_RECTANGLE,
	BRUSH_SHAPE_CIRCLE
};

enum BrushAction {
	BRUSH_ACTION_DRAW,
	BRUSH_ACTION_ERASE
};

enum BrushFill {
	BRUSH_FILL_GROUND,
	BRUSH_FILL_WALL,
	BRUSH_FILL_AREA,
	BRUSH_FILL_ENTITY
};

struct BrushPreview {
	std::map<Coord, GroundId> grounds;
	std::map<WallCoord, WallId> walls;
	std::map<Coord, EntityTemplateId> entities;
};

class Brush {
public:
	Brush(const EntityTemplateDb& template_db);

	void apply(Labyrinth& labyrinth, float x, float y) const;
	BrushPreview preview(const Labyrinth& labyrinth, float x, float y) const;

	GroundId getGroundId() const { return ground_id; }
	void setGroundId(GroundId ground_id_init) { ground_id = ground_id_init; }

	WallId getWallId() const { return wall_id; }
	void setWallId(WallId wall_id_init) { wall_id = wall_id_init; }

	EntityTemplateId getEntityId() const { return entity_id; }
	void setEntityId(EntityTemplateId entity_id_init) { entity_id = entity_id_init; }

	BrushShape getBrushShape() const { return brush_type; }
	void setBrushShape(BrushShape type) { brush_type = type; }

	BrushAction getBrushAction() const { return brush_action; }
	void setBrushAction(BrushAction type) { brush_action = type; }

	BrushFill getBrushFill() const { return brush_fill; }
	void setBrushFill(BrushFill type) { brush_fill = type; }
private:
	void applyPreview(Labyrinth& labyrinth, const BrushPreview& brush_preview) const;
	
	BrushPreview pointPreview(const Labyrinth& labyrinth, float x, float y) const;
	void enclosingWall(BrushPreview& preview, const Labyrinth& labyrinth, Coord coord, CardinalDirection d) const;

	BrushPreview wallPreview(const Labyrinth& labyrinth, float x, float y) const;
	bool isMin4(float x, float o1, float o2, float o3) const;

	BrushShape brush_type = BRUSH_SHAPE_POINT;
	BrushAction brush_action = BRUSH_ACTION_DRAW;
	BrushFill brush_fill = BRUSH_FILL_AREA;

	GroundId ground_id = 1;
	WallId wall_id = 1;
	EntityTemplateId entity_id = 1;

	const EntityTemplateDb& template_db;
	float wall_dead_zone = 0.3f;
};