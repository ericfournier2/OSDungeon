#include "imgui/imgui-SFML.h"
#include "imgui_utils.h"
#include "view_utils.h"

bool textureSelect(const std::string& label, TextureId* id, const TextureDb& tdb) {
	std::string popup_label = label;
	popup_label.append("_popup");
	TextureInfo tex_info = tdb.getTexture(*id);
	sf::Vector2f tex_size = { 100.0f, 100.0f };
	if (ImGui::ImageButton(label.c_str(), *(tex_info.texture.get()), tex_size, sf::Color::Black)) {
		ImGui::OpenPopup(popup_label.c_str());
	}

	bool retval = false;
	if (ImGui::BeginPopup(popup_label.c_str())) {
		auto all_ids = tdb.getIds();
		for (int i = 0; i < all_ids.size(); ++i) {
			auto current_id = all_ids[i];
			TextureInfo info = tdb.getTexture(current_id);
			std::string selectable_label = info.name;
			selectable_label.append("###");
			selectable_label.append(std::to_string(current_id));
			if (ImGui::Selectable(selectable_label.c_str())) {
				*id = current_id;
				retval = true;
			}
		}
		ImGui::EndPopup();
	}

	return retval;
}

AnimatedEntity::AnimatedEntity(const SpriteInfo& info_, const TextureDb& tdb_, RelativeDirection facing_, bool loop_) 
	: info(info_), tdb(tdb_), loop(loop_), facing(facing_) 
{
}

sf::Sprite AnimatedEntity::getSprite(int size_x, int size_y) {
	sf::Sprite sprite = getAnimationSprite(info, facing, clock, tdb);
	
	sf::Vector2f sprite_size = sprite.getLocalBounds().size;
	float x_scale = (float) size_x / sprite_size.x;
	float y_scale = (float) size_y / sprite_size.y;

	//sprite.setPosition({ (float)-size_x, 0.0f });
	sprite.setScale({ x_scale, y_scale });

	return sprite;
}

template <typename TId, typename TInfo, typename TDb>
bool selectionWidget(TId* id, TDb& db, const std::string& display_name, bool add_button = false) {
	bool retval = false;

	// Add button.
	if (add_button) {
		std::string button_name = "+##Add ";
		button_name.append(display_name);
		if (ImGui::Button(button_name.c_str())) {
			TInfo info = TInfo();
			*id = db.addElement(info);
			info = db.getElement(*id);

			info.name = "New ";
			info.name.append(display_name + " (" + std::to_string(*id) + ")");
			db.updateElement(info);

			retval = true;
		}
		ImGui::SameLine();
	}

	// Sprite name select combo.
	std::string combo_display = "Select a ";
	combo_display.append(display_name + " to start...");
	if (*id != 0) {
		combo_display = db.getElement(*id).name;
	}
	std::string combo_name = "Select ";
	combo_name.append(display_name);
	if (ImGui::BeginCombo(combo_name.c_str(), combo_display.c_str())) {
		auto id_list = db.getIds();
		for (const auto& current_id : id_list) {
			std::string item_label = db.getElement(current_id).name;
			item_label.append("##");
			item_label.append(std::to_string(current_id));
			if (ImGui::Selectable(item_label.c_str())) {
				*id = current_id;
				retval = true;
			}
		}
		ImGui::EndCombo();
	}

	return retval;
}

bool entityTemplateSelectionWidget(EntityTemplateId* id, EntityTemplateDb& edb, bool add_button) {
	return selectionWidget<EntityTemplateId, EntityTemplateInfo, EntityTemplateDb>(id, edb, "entity template", add_button);
	return false;
}

bool spriteSelectionWidget(SpriteId* id, SpriteDb& sdb, bool add_button) {
	return selectionWidget<SpriteId, SpriteInfo, SpriteDb>(id, sdb, "sprite", add_button);
}
