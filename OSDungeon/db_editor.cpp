#include "db_editor.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

DatabaseEditor::DatabaseEditor(Databases& db_init)
 : db(db_init) 
{
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
	
	std::string texture_label = "texture_button##";
	texture_label.append(std::to_string(object_id));
	texture_label.append(extra_label);
	std::string popup_label = "texture_popup##";
	popup_label.append(std::to_string(object_id));
	popup_label.append(extra_label);

	TextureInfo info = db.tdb.getTexture(*texture_id);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	
	if (ImGui::ImageButton(texture_label.c_str(), *(info.texture.get()), tex_size)) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = db.tdb.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			std::string selectable_label = db.tdb.getTexture(all_ids[i]).texture_filename;
			selectable_label.append("##");
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
	auto wall_info = db.wdb.getElement(id);
	if (selectColorButton(&(wall_info.color), id)) {
		db.wdb.updateElement(wall_info);
	}

	ImGui::TableNextColumn();
	if (selectTextureButton(&(wall_info.texture), id)) {
		db.wdb.updateElement(wall_info);
	}
}

void DatabaseEditor::renderWallTable() {
	if (ImGui::BeginTable("Wall entries", 3, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("id", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Color", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
		auto all_ids = db.wdb.getIds();
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
	auto ground_info = db.gdb.getElement(id);
	if (selectColorButton(&(ground_info.ground_color), id, "ground")) {
		db.gdb.updateElement(ground_info);
	}

	ImGui::TableNextColumn();
	if (selectColorButton(&(ground_info.ceiling_color), id, "ceiling")) {
		db.gdb.updateElement(ground_info);
	}

	ImGui::TableNextColumn();
	if (selectTextureButton(&(ground_info.texture), id)) {
		db.gdb.updateElement(ground_info);
	}
}

void DatabaseEditor::renderGroundTable() {
	if (ImGui::BeginTable("Ground entries", 4, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("id", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Ceiling", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Ground", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
		auto all_ids = db.gdb.getIds();
		for (auto id : all_ids) {
			renderGroundRow(id);
		}
		ImGui::EndTable();
	}
}

std::string labelWithId(const std::string& label, int id) {
	std::string retVal = label;
	retVal.append("##");
	retVal.append(std::to_string(id));
	return retVal;
}

void floatColumn(std::string label, float* val, EntityTemplateId id, EntityTemplateInfo* info, EntityTemplateDb& template_db) {
	ImGui::TableNextColumn();
	std::string full_label = labelWithId(label, id);
	if (ImGui::InputFloat(full_label.c_str(), val)) {
		template_db.updateElement(*info);
	}

}

void DatabaseEditor::renderEntityRow(EntityTemplateId id) {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	std::string id_str = std::to_string(id);
	ImGui::Text(id_str.c_str());

	auto template_info = db.edb.getElement(id);
	floatColumn("##x_size", &template_info.x_size, id, &template_info, db.edb);
	floatColumn("##y_size", &template_info.y_size, id, &template_info, db.edb);
	floatColumn("##x_offset", &template_info.x_offset, id, &template_info, db.edb);
	floatColumn("##y_offset", &template_info.y_offset, id, &template_info, db.edb);

	ImGui::TableNextColumn();
	if (selectTextureButton(&(template_info.texture), id)) {
		db.edb.updateElement(template_info);
	}
}

void DatabaseEditor::renderEntityTable() {
	if (ImGui::BeginTable("Entity templates", 7, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		ImGui::TableSetupColumn("id", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("x size", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("y size", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("x offset", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("y offset", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Texture", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableHeadersRow();
		auto all_ids = db.edb.getIds();
		for (auto id : all_ids) {
			renderEntityRow(id);
		}
		ImGui::EndTable();
	}
}

void DatabaseEditor::renderTextureRow(TextureId id) {
	ImGui::TableNextRow();
	ImGui::TableNextColumn();
	std::string id_str = std::to_string(id);
	ImGui::Text(id_str.c_str());

	ImGui::TableNextColumn();
	auto texture_info = db.tdb.getTexture(id);
	ImGui::Text(texture_info.texture_filename.c_str());

	ImGui::TableNextColumn();
	ImGui::Image(*texture_info.texture.get(), { 100.0f, 100.0f });
}

void DatabaseEditor::renderTextureTable() {
	if (ImGui::BeginTable("Texture entries", 3, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_SizingStretchProp)) {
		auto all_ids = db.tdb.getIds();
		for (auto id : all_ids) {
			renderTextureRow(id);
		}
		ImGui::EndTable();
	}
}

void DatabaseEditor::render() {
	ImGui::Begin("Database editor");
	
	if (ImGui::BeginTabBar("Databases", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Walls")) {
			if (ImGui::Button("Add")) {
				db.wdb.addElement(WallInfo());
			}
			renderWallTable();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Ground")) {
			if (ImGui::Button("Add")) {
				db.gdb.addElement(GroundInfo());
			}
			renderGroundTable();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Textures")) {
			if (ImGui::Button("Add")) {
				//texture_db.addTexture()
			}
			renderTextureTable();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Entities")) {
			if (ImGui::Button("Add")) {
				db.edb.addElement(EntityTemplateInfo());
			}
			renderEntityTable();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}

	ImGui::End();
}