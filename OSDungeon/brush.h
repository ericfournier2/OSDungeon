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

enum BrushFillType {
	BRUSH_FILL_GROUND,
	BRUSH_FILL_WALL,
	BRUSH_FILL_AREA
};

struct BrushPreview {
	std::map<Coord, GroundTypeId> grounds;
	std::map<WallCoord, WallTypeId> walls;
	std::map<Coord, EntityTemplateId> entities;
};

class Brush {
public:
	void apply(Labyrinth& labyrinth, float x, float y) const;
	BrushPreview preview(const Labyrinth& labyrinth, float x, float y) const;

	GroundTypeId getGroundId() const { return ground_id; }
	void setGroundId(GroundTypeId ground_id_init) { ground_id = ground_id_init; }

	WallTypeId getWallId() const { return wall_id; }
	void setWallId(WallTypeId wall_id_init) { wall_id = wall_id_init; }

	BrushShape getBrushType() const { return brush_type; }
	void setBrushType(BrushShape type) { brush_type = type; }

	BrushAction getBrushAction() const { return brush_action; }
	void setBrushAction(BrushAction type) { brush_action = type; }

	BrushFillType getBrushFillType() const { return brush_fill; }
	void setBrushFillType(BrushFillType type) { brush_fill = type; }
private:
	void applyPreview(Labyrinth& labyrinth, const BrushPreview& brush_preview) const;
	
	BrushPreview pointPreview(const Labyrinth& labyrinth, float x, float y) const;
	void enclosingWall(BrushPreview& preview, const Labyrinth& labyrinth, Coord coord, CardinalDirection d) const;


	BrushShape brush_type = BRUSH_SHAPE_POINT;
	BrushAction brush_action = BRUSH_ACTION_DRAW;
	BrushFillType brush_fill = BRUSH_FILL_AREA;

	GroundTypeId ground_id = 1;
	WallTypeId wall_id = 1;
};