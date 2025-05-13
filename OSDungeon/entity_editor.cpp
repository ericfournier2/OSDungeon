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

	return ImGui::SliderInt(label.c_str(), value, 0, enum_names.size() - 1, display_name.c_str(), ImGuiSliderFlags_NoInput); // Use ImGuiSliderFlags_NoInput flag to disable CTRL+Click here.
}

std::string tileVecToString(const TileVec& tiles) {
	std::ostringstream oss;
	for (const auto& tile : tiles) {
		oss << tile << ';';
	}

	return oss.str();
}

void EntityEditor::renderSelectionWidget() {
	// Add button.
	if (ImGui::Button("+##Add entity")) {
		current_info = EntityTemplateInfo();
		current_info.name = "New entity";
		current_id = db.edb.addElement(current_info);
		current_info.id = current_id;
		current_info.sprite_id = 0;
		current_info.name.append(std::to_string(current_id));
		db.edb.updateElement(current_info);
	}
	ImGui::SameLine();

	// Entity name select combo.
	std::string combo_display = "Select an entity to start...";
	if (current_id != 0) {
		combo_display = current_info.name;
	}
	if (ImGui::BeginCombo("Select entity", combo_display.c_str())) {
		auto id_list = db.edb.getIds();
		//std::map<EntityTemplateId, std::string> entity_names;
		for (const auto& id : id_list) {
			//entity_names.emplace(id, db.edb.getElement(id).name);
			std::string item_label = db.edb.getElement(id).name;
			item_label.append("##");
			item_label.append(std::to_string(id));
			if (ImGui::Selectable(item_label.c_str())) {
				current_id = id;
				current_info = db.edb.getElement(id);
				sprite_preview = std::unique_ptr<AnimatedEntity>(new AnimatedEntity(db.sdb.getElement(current_info.sprite_id), db.tdb));
			}
		}
		ImGui::EndCombo();
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

		} catch (std::invalid_argument const& ex) {
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
	if (ImGui::Begin("Entity editor", NULL, 0)) {
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
	animated_entities.emplace(RelativeDirection::FRONT, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::FRONT));
	animated_entities.emplace(RelativeDirection::BACK, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::BACK));
	animated_entities.emplace(RelativeDirection::LEFT, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::LEFT));
	animated_entities.emplace(RelativeDirection::RIGHT, AnimatedEntity(current_sprite, db.tdb, RelativeDirection::RIGHT));
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
	// Add button.
	if (ImGui::Button("+##Add sprite")) {
		current_sprite = SpriteInfo();
		current_sprite.name = "New sprite (";
		auto sprite_id = db.sdb.addElement(current_sprite);
		current_sprite = db.sdb.getElement(sprite_id);
		
		current_sprite.name.append(std::to_string(sprite_id));
		current_sprite.name.append(")");
		db.sdb.updateElement(current_sprite);
		refreshStrings();
		animated_entities.clear();
	}
	ImGui::SameLine();

	// Sprite name select combo.
	std::string combo_display = "Select a sprite to start...";
	if (current_sprite.id != 0) {
		combo_display = current_sprite.name;
	}
	if (ImGui::BeginCombo("Select sprite", combo_display.c_str())) {
		auto id_list = db.sdb.getIds();
		for (const auto& id : id_list) {
			std::string item_label = db.sdb.getElement(id).name;
			item_label.append("##");
			item_label.append(std::to_string(id));
			if (ImGui::Selectable(item_label.c_str())) {
				current_sprite = db.sdb.getElement(id);
				refreshStrings();
				refreshAnimations();
			}
		}
		ImGui::EndCombo();
	}
}