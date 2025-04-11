#include "wall_db_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

DatabaseEditor::DatabaseEditor(WallDb& wall_db_init, GroundDb& ground_db_init, TextureDb& texture_db_init) 
 : wall_db(wall_db_init), ground_db(ground_db_init), texture_db(texture_db_init), window(sf::VideoMode({ 800, 400 }), "Databases") {
	ImGui::SFML::Init(window);
}

bool DatabaseEditor::selectColorButton(sf::Color* initial_color, int object_id, std::string extra_label) {
	// Get the unsigned integers into equivalent floats.
	float color[4] = { initial_color->r / 255.0f, initial_color->g / 255.0f, initial_color->b / 255.0f, initial_color->a / 255.0f };
	
	std::string color_label = "color_button###";
	color_label.append(std::to_string(object_id));
	color_label.append(extra_label);

	if (ImGui::ColorEdit4(color_label.c_str(), color, ImGuiColorEditFlags_Uint8 | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
		initial_color->r = static_cast<uint8_t>(std::lround(color[0] * 255));
		initial_color->g = static_cast<uint8_t>(std::lround(color[1] * 255));
		initial_color->b = static_cast<uint8_t>(std::lround(color[2] * 255));
		initial_color->a = static_cast<uint8_t>(std::lround(color[3] * 255));

		return true;
	}

	return false;
}

bool DatabaseEditor::selectTextureButton(TextureId* texture_id, int object_id, std::string extra_label) {
	bool retval = false;
	
	std::string texture_label = "texture_button###";
	texture_label.append(std::to_string(object_id));
	texture_label.append(extra_label);
	std::string popup_label = "texture_popup###";
	popup_label.append(std::to_string(object_id));
	popup_label.append(extra_label);

	TextureInfo info = texture_db.getTexture(*texture_id);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(texture_label.c_str(), *(info.texture.get()), tex_size)) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = texture_db.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			std::string selectable_label = texture_db.getTexture(all_ids[i]).texture_filename;
			selectable_label.append("###");
			selectable_label.append(std::to_string(i));
			if (ImGui::Selectable(selectable_label.c_str())) {
				*texture_id = all_ids[i];
				retval = true;
			}
		}
		ImGui::EndPopup();
	}

	return retval;
}

void DatabaseEditor::renderWallRow(WallTypeId id) {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	std::string id_str = std::to_string(id);
	ImGui::Text(id_str.c_str());

	ImGui::TableNextColumn();
	auto wall_info = wall_db.getElement(id);
	if (selectColorButton(&(wall_info.color), id)) {
		wall_db.updateElement(wall_info);
	}

	ImGui::TableNextColumn();
	if (selectTextureButton(&(wall_info.texture), id)) {
		wall_db.updateElement(wall_info);
	}
}

void DatabaseEditor::renderWallTable() {
	if (ImGui::BeginTable("Wall entries", 3, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		auto all_ids = wall_db.getIds();
		for (auto id : all_ids) {
			renderWallRow(id);
		}
		ImGui::EndTable();
	}
}

void DatabaseEditor::renderGroundRow(GroundTypeId id) {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	std::string id_str = std::to_string(id);
	ImGui::Text(id_str.c_str());

	ImGui::TableNextColumn();
	auto ground_info = ground_db.getElement(id);
	if (selectColorButton(&(ground_info.ground_color), id, "ground")) {
		ground_db.updateElement(ground_info);
	}

	ImGui::TableNextColumn();
	if (selectColorButton(&(ground_info.ceiling_color), id, "ceiling")) {
		ground_db.updateElement(ground_info);
	}

	ImGui::TableNextColumn();
	if (selectTextureButton(&(ground_info.texture), id)) {
		ground_db.updateElement(ground_info);
	}
}

void DatabaseEditor::renderGroundTable() {
	if (ImGui::BeginTable("Ground entries", 4, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		auto all_ids = ground_db.getIds();
		for (auto id : all_ids) {
			renderGroundRow(id);
		}
		ImGui::EndTable();
	}
}

void DatabaseEditor::render() {
	window.clear();
	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::Begin("Database editor");
	
	if (ImGui::BeginTabBar("Databases", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Walls")) {
			renderWallTable();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Ground")) {
			renderGroundTable();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Textures")) {
			ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::SFML::Render(window);
	window.display();
}

bool DatabaseEditor::processEvents() {
	while (const std::optional event = window.pollEvent()) {
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>()) {
			window.close();
			return true;
		}
	}
}