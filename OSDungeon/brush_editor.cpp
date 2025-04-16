#include "brush_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

BrushEditor::BrushEditor(Brush& brush_init, const GroundDb& ground, const WallDb& wall, const TextureDb& texture) 
	: brush(brush_init), ground_db(ground), wall_db(wall), texture_db(texture)
{
}

sf::Color getTextureButtonColor(WallInfo info) {
	return info.color;
}

sf::Color getTextureButtonColor(GroundInfo info) {
	return info.ground_color;
}

template <typename TDb>
auto brushPopUp(const std::string& popup_label, typename TDb::IdType* id, const TDb& db, const TextureDb& texture_db) {
	typename TDb::InfoType info = db.getElement(*id);
	std::string texture_label = popup_label;
	texture_label.append("_texture");
	TextureInfo tex_info = texture_db.getTexture(info.texture);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(texture_label.c_str(), *(tex_info.texture.get()), tex_size, sf::Color::Black, getTextureButtonColor(info))) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	bool retval = false;
	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = db.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			std::string selectable_label = std::to_string(db.getElement(all_ids[i]).id);
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
	ImGui::SeparatorText("Wall");
	WallTypeId wall_id = brush.getWallId();
	if (brushPopUp<WallDb>("Wall brush", &wall_id, wall_db, texture_db)) {
		brush.setWallId(wall_id);
	}
	ImGui::SeparatorText("Ground");
	GroundTypeId ground_id = brush.getGroundId();
	if (brushPopUp<GroundDb>("Ground brush", &ground_id, ground_db, texture_db)) {
		brush.setGroundId(ground_id);
	}
	ImGui::End();
}