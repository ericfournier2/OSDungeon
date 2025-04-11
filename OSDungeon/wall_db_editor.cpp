#include "wall_db_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

WallDbEditor::WallDbEditor(WallDb& wall_db_init, TextureDb& texture_db_init) 
 : wall_db(wall_db_init), texture_db(texture_db_init), window(sf::VideoMode({ 800, 400 }), "Wall DB") {
	ImGui::SFML::Init(window);
}

void WallDbEditor::updateWall() {
	wall_db.updateElement(WallInfo({ current_id, current_color, current_texture }));
}

void WallDbEditor::render() {
	window.clear();
	ImGui::SFML::Update(window, deltaClock.restart());

	ImGui::Begin("Wall database");
	if (ImGui::BeginTable("Wall entries", 3, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		auto all_ids = wall_db.getIds();
		for (auto id : all_ids) {
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			std::string id_str = std::to_string(id);
			ImGui::Text(id_str.c_str());

			ImGui::TableNextColumn();
			auto wall_info = wall_db.getElement(id);
			auto wall_color = wall_info.color;
			float color[4] = { wall_color.r / 255.0f, wall_color.g / 255.0f, wall_color.b / 255.0f, wall_color.a / 255.0f};
			std::string color_label = "Wall color###";
			color_label.append(std::to_string(id));
			if (ImGui::ColorEdit4(color_label.c_str(), color, ImGuiColorEditFlags_Uint8)) {
				wall_color.r = static_cast<uint8_t>(std::lround(color[0] * 255));
				wall_color.g = static_cast<uint8_t>(std::lround(color[1] * 255));
				wall_color.b = static_cast<uint8_t>(std::lround(color[2] * 255));
				wall_color.a = static_cast<uint8_t>(std::lround(color[3] * 255));

				wall_info.color = wall_color;
				wall_db.updateElement(wall_info);

			}

			ImGui::TableNextColumn();
			sf::Vector2f tex_size = { 100.0f, 100.0f };
			ImGui::Image(*(texture_db.getTexture(wall_info.texture).texture.get()), tex_size);
		}
		ImGui::EndTable();
	}
	/*
	int slider_value = static_cast<int>(current_id);
	if (ImGui::SliderInt("ID", &slider_value, 0, 5)) {
		current_id = slider_value;
	}
	float color[4] = { current_color.r, current_color.g, current_color.b, current_color.a };
	if (ImGui::ColorEdit4("Wall color", color)) {
		current_color.r = color[0];
		current_color.g = color[1];
		current_color.b = color[2];
		current_color.a = color[3];

		updateWall();
	};*/
	ImGui::End();
	ImGui::SFML::Render(window);
	window.display();
}

bool WallDbEditor::processEvents() {
	while (const std::optional event = window.pollEvent()) {
		ImGui::SFML::ProcessEvent(window, *event);
		if (event->is<sf::Event::Closed>()) {
			window.close();
			return true;
		}
	}
}