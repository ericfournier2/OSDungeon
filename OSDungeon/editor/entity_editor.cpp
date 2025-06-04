#include <sstream> 
#include "entity_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui_stdlib.h"
#include "imgui_utils.h"

EntityEditor::EntityEditor(Databases& db_) 
	: db(db_)
{}

bool enumSlider(std::string label, int* value, std::vector<std::string> enum_names) {
	std::string display_name = "Unknown";
	if (*value < enum_names.size()) {
		display_name = std::string(enum_names[*value]);
	}

	return ImGui::SliderInt(label.c_str(), value, 0, static_cast<int>(enum_names.size() - 1), display_name.c_str(), ImGuiSliderFlags_NoInput); // Use ImGuiSliderFlags_NoInput flag to disable CTRL+Click here.
}

std::string tileVecToString(const TileVec& tiles) {
	std::ostringstream oss;
	for (const auto& tile : tiles) {
		oss << tile << ';';
	}

	return oss.str();
}

void EntityEditor::renderSelectionWidget() {
	if (entityTemplateSelectionWidget(&current_id, db.edb, true)) {
		current_info = db.edb.getElement(current_id);
		sprite_preview = std::unique_ptr<AnimatedEntity>(new AnimatedEntity(db.sdb.getElement(current_info.sprite_id), db.tdb));
	}
}

TileVec stringToTileVec(const std::string& str) {
	TileVec retval;
	std::istringstream iss(str);
	std::string last_element;
	while (std::getline(iss, last_element, ';')) {
		try {
			int next_frame = std::stoi(last_element);
			retval.push_back(next_frame);

		} catch (std::invalid_argument const&) {
			retval.push_back(0);
		}
	}

	return retval;
}

int tileVecFilter(ImGuiInputTextCallbackData* data)
{
	if ((data->EventChar >= '0' && data->EventChar <= '9') || (data->EventChar == ';') || (data->EventChar == '-')) {
		return 0;
	}

	return 1;
}

void EntityEditor::render() {
	if (ImGui::Begin("Entity template editor", NULL, 0)) {
		renderSelectionWidget();

		if (current_id != 0) {
			bool update = false;
			update = ImGui::InputText("Name", &current_info.name);
			static std::vector<std::string> movement_names({ "Static", "Wander", "Follow", "Flee", "Fixed path" });
			update = enumSlider("Movement type", reinterpret_cast<int*>(&current_info.movement), movement_names) || update;
			static std::vector<std::string> collision_names({ "None", "Pickable", "Dialog", "Lethal" });
			update = enumSlider("Collision type", reinterpret_cast<int*>(&current_info.collision), collision_names) || update;
			static std::vector<std::string> interaction_names({ "None", "Dialog" });
			update = enumSlider("Interaction type", reinterpret_cast<int*>(&current_info.interaction), interaction_names) || update;
			if (spriteSelectionWidget(&current_info.sprite_id, db.sdb)) {
				update = true;
				sprite_preview = std::unique_ptr<AnimatedEntity>(new AnimatedEntity(db.sdb.getElement(current_info.sprite_id), db.tdb));
			}
			if (current_info.sprite_id != 0) {
				ImGui::Image(sprite_preview->getSprite(100, 100));
			} else {
				ImGui::Text("No sprite selected.");
			}

			if (update) {
				db.edb.updateElement(current_info);
			}
		}
	}
	ImGui::End();
}

SpriteEditor::SpriteEditor(Databases& db_)
	: db(db_)
{
}

void SpriteEditor::refreshStrings() {
	tile_vec_string_front = tileVecToString(current_sprite.front);
	tile_vec_string_back = tileVecToString(current_sprite.back);
	tile_vec_string_left = tileVecToString(current_sprite.left);
	tile_vec_string_right = tileVecToString(current_sprite.right);
}

void SpriteEditor::refreshAnimations() {
	animated_entities.clear();
	if (current_sprite.texture != 0) {
		animated_entities.emplace(RelativeDirection::FRONT, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::FRONT));
		animated_entities.emplace(RelativeDirection::BACK, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::BACK));
		animated_entities.emplace(RelativeDirection::LEFT, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::LEFT));
		animated_entities.emplace(RelativeDirection::RIGHT, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::RIGHT));
	}
}

void SpriteEditor::render() {
	if (ImGui::Begin("Sprite editor", NULL, 0)) {
		renderSelectionWidget();

		if (current_sprite.id != 0) {
			bool update = false;
			update = ImGui::InputText("Name", &current_sprite.name);
			if (textureSelect("Texture", &current_sprite.texture, db.tdb)) {
				update = true;

			} else if (current_sprite.texture != 0) {
				update = ImGui::InputText("Front", &tile_vec_string_front, ImGuiInputTextFlags_CallbackCharFilter, tileVecFilter) || update;
				ImGui::Image(animated_entities.at(RelativeDirection::FRONT).getSprite(100, 100));
				update = ImGui::InputText("Back", &tile_vec_string_back, ImGuiInputTextFlags_CallbackCharFilter, tileVecFilter) || update;
				ImGui::Image(animated_entities.at(RelativeDirection::BACK).getSprite(100, 100));
				update = ImGui::InputText("Left", &tile_vec_string_left, ImGuiInputTextFlags_CallbackCharFilter, tileVecFilter) || update;
				ImGui::Image(animated_entities.at(RelativeDirection::LEFT).getSprite(100, 100));
				update = ImGui::InputText("Right", &tile_vec_string_right, ImGuiInputTextFlags_CallbackCharFilter, tileVecFilter) || update;
				ImGui::Image(animated_entities.at(RelativeDirection::RIGHT).getSprite(100, 100));
			}

			if (update) {
				current_sprite.front = stringToTileVec(tile_vec_string_front);
				current_sprite.back = stringToTileVec(tile_vec_string_back);
				current_sprite.left = stringToTileVec(tile_vec_string_left);
				current_sprite.right = stringToTileVec(tile_vec_string_right);
				
				db.sdb.updateElement(current_sprite);
				refreshAnimations();
			}
		} else {
			ImGui::Text("No texture selected.");
		}
	}
	ImGui::End();
}

void SpriteEditor::renderSelectionWidget() {
	if (spriteSelectionWidget(&current_sprite.id, db.sdb, true)) {
		current_sprite = db.sdb.getElement(current_sprite.id);
		refreshStrings();
		refreshAnimations();
	}
}