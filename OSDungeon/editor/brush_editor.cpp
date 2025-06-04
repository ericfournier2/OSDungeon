#include "brush_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "imgui_utils.h"

BrushEditor::BrushEditor(Brush& brush_init, const Databases& db_init)
	: brush(brush_init), db(db_init)
{
}

sf::Color getTextureButtonColor(WallInfo info) {
	return info.color;
}

sf::Color getTextureButtonColor(GroundInfo info) {
	return info.ground_color;
}

sf::Color getTextureButtonColor(EntityTemplateInfo info) {
	return sf::Color::White;
}

void BrushEditor::render() {
	ImGui::Begin("Brush");
	ImGui::SeparatorText("Action");
	BrushAction action = brush.getBrushAction();
	if (ImGui::RadioButton("Draw", action == BrushAction::BRUSH_ACTION_DRAW)) {
		brush.setBrushAction(BrushAction::BRUSH_ACTION_DRAW);
	}
	if (ImGui::RadioButton("Erase", action == BrushAction::BRUSH_ACTION_ERASE)) {
		brush.setBrushAction(BrushAction::BRUSH_ACTION_ERASE);
	}

	ImGui::SeparatorText("Shape");
	BrushShape shape = brush.getBrushShape();
	if (ImGui::RadioButton("Wall##Shape", shape == BrushShape::BRUSH_SHAPE_WALL)) {
		brush.setBrushShape(BrushShape::BRUSH_SHAPE_WALL);
	}
	if (ImGui::RadioButton("Point##Shape", shape == BrushShape::BRUSH_SHAPE_POINT)) {
		brush.setBrushShape(BrushShape::BRUSH_SHAPE_POINT);
	}

	ImGui::SeparatorText("Fill");
	BrushFill fill = brush.getBrushFill();
	if (ImGui::RadioButton("Wall##Fill", fill == BrushFill::BRUSH_FILL_WALL)) {
		brush.setBrushFill(BrushFill::BRUSH_FILL_WALL);
	}
	if (ImGui::RadioButton("Ground##Fill", fill == BrushFill::BRUSH_FILL_GROUND)) {
		brush.setBrushFill(BrushFill::BRUSH_FILL_GROUND);
	}
	if (ImGui::RadioButton("Area##Fill", fill == BrushFill::BRUSH_FILL_AREA)) {
		brush.setBrushFill(BrushFill::BRUSH_FILL_AREA);
	}
	if (ImGui::RadioButton("Entity##Fill", fill == BrushFill::BRUSH_FILL_ENTITY)) {
		brush.setBrushFill(BrushFill::BRUSH_FILL_ENTITY);
	}
	if (ImGui::RadioButton("Entity (Fixed)##Fill", fill == BrushFill::BRUSH_FILL_ENTITY_FIXED)) {
		brush.setBrushFill(BrushFill::BRUSH_FILL_ENTITY_FIXED);
	}

	ImGui::SeparatorText("Wall");
	WallId wall_id = brush.getWallId();
	if (imageSelectPopUp<WallDb>("Wall brush", &wall_id, db.wdb, db.tdb, db.sdb)) {
		brush.setWallId(wall_id);
	}

	ImGui::SeparatorText("Ground");
	GroundId ground_id = brush.getGroundId();
	if (imageSelectPopUp<GroundDb>("Ground brush", &ground_id, db.gdb, db.tdb, db.sdb)) {
		brush.setGroundId(ground_id);
	}

	ImGui::SeparatorText("Entity");
	EntityTemplateId entity_id = brush.getEntityId();
	if (imageSelectPopUp<EntityTemplateDb>("Entity brush", &entity_id, db.edb, db.tdb, db.sdb)) {
		brush.setEntityId(entity_id);
	}
	ImGui::End();
}