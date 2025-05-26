#include "brush_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

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

TextureInfo getTextureFromInfo(const GroundInfo& info, const TextureDb& texture_db, const SpriteDb& sprite_db) {
	return texture_db.getTexture(info.texture);
}

TextureInfo getTextureFromInfo(const WallInfo& info, const TextureDb& texture_db, const SpriteDb& sprite_db) {
	return texture_db.getTexture(info.front.texture);
}

TextureInfo getTextureFromInfo(const EntityTemplateInfo& info, const TextureDb& texture_db, const SpriteDb& sprite_db) {
	return texture_db.getTexture(sprite_db.getElement(info.sprite_id).texture);
}

template <typename TDb>
auto brushPopUp(const std::string& popup_label, typename TDb::IdType* id, const TDb& db, const TextureDb& texture_db, const SpriteDb& sprite_db) {
	typename TDb::InfoType info = db.getElement(*id);
	std::string texture_label = popup_label;
	texture_label.append("_texture");
	TextureInfo tex_info = getTextureFromInfo(info, texture_db, sprite_db);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(texture_label.c_str(), *(tex_info.texture.get()), tex_size, sf::Color::Black, getTextureButtonColor(info))) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	bool retval = false;
	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = db.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			std::string selectable_label = db.getElement(all_ids[i]).name;
			selectable_label.append("###");
			selectable_label.append(std::to_string(i));
			if (ImGui::Selectable(selectable_label.c_str())) {
				*id = all_ids[i];
				retval = true;
			}
		}
		ImGui::EndPopup();
	}

	return retval;
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
	if (brushPopUp<WallDb>("Wall brush", &wall_id, db.wdb, db.tdb, db.sdb)) {
		brush.setWallId(wall_id);
	}

	ImGui::SeparatorText("Ground");
	GroundId ground_id = brush.getGroundId();
	if (brushPopUp<GroundDb>("Ground brush", &ground_id, db.gdb, db.tdb, db.sdb)) {
		brush.setGroundId(ground_id);
	}

	ImGui::SeparatorText("Entity");
	EntityTemplateId entity_id = brush.getEntityId();
	if (brushPopUp<EntityTemplateDb>("Entity brush", &entity_id, db.edb, db.tdb, db.sdb)) {
		brush.setEntityId(entity_id);
	}
	ImGui::End();
}