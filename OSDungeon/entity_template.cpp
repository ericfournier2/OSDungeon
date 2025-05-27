#include "entity_template.h"

bool EntityTemplateInfo::write(std::ofstream& stream) const {
	stream.write(reinterpret_cast<const char*>(&id), sizeof(id));
	stream.write(reinterpret_cast<const char*>(&sprite_id), sizeof(sprite_id));
	stream << name << '\0';
	stream.write(reinterpret_cast<const char*>(&movement), sizeof(movement));
	stream.write(reinterpret_cast<const char*>(&collision), sizeof(collision));
	stream.write(reinterpret_cast<const char*>(&x_size), sizeof(float));
	stream.write(reinterpret_cast<const char*>(&y_size), sizeof(float));
	stream.write(reinterpret_cast<const char*>(&cast_shadow), sizeof(bool));

	return !stream.fail();
}

bool EntityTemplateInfo::read(std::ifstream& stream) {
	stream.read(reinterpret_cast<char*>(&id), sizeof(id));
	stream.read(reinterpret_cast<char*>(&sprite_id), sizeof(sprite_id));
	std::getline(stream, name, '\0');
	stream.read(reinterpret_cast<char*>(&movement), sizeof(movement));
	stream.read(reinterpret_cast<char*>(&collision), sizeof(collision));
	stream.read(reinterpret_cast<char*>(&x_size), sizeof(float));
	stream.read(reinterpret_cast<char*>(&y_size), sizeof(float));
	stream.read(reinterpret_cast<char*>(&cast_shadow), sizeof(bool));

	return !stream.fail();
}
