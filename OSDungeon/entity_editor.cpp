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
				tile_vec_string_front = tileVecToString(current_info.front);
				tile_vec_string_back = tileVecToString(current_info.back);
				tile_vec_string_left = tileVecToString(current_info.left);
				tile_vec_string_right = tileVecToString(current_info.right);
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
			update = textureSelect("Texture", &current_info.texture, db.tdb) || update;
			update = ImGui::InputText("Front", &tile_vec_string_front, 0, tileVecFilter ) || update;
			update = ImGui::InputText("Back", &tile_vec_string_back, 0, tileVecFilter) || update;
			update = ImGui::InputText("Left", &tile_vec_string_left, 0, tileVecFilter) || update;
			update = ImGui::InputText("Right", &tile_vec_string_right, 0, tileVecFilter) || update;

			if (update) {
				current_info.front = stringToTileVec(tile_vec_string_front);
				current_info.back = stringToTileVec(tile_vec_string_back);
				current_info.left = stringToTileVec(tile_vec_string_left);
				current_info.right = stringToTileVec(tile_vec_string_right);

				db.edb.updateElement(current_info);
			}

		}
	}
	ImGui::End();
}