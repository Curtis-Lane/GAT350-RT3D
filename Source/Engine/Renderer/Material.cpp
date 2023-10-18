#include "Material.h"

#include "Core/Core.h"
#include "Program.h"
#include "Texture.h"

namespace nc {
	bool Material::Create(std::string filename, ...) {
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if(!success) {
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		this->program = GET_RESOURCE(Program, program);

		// read the textures name
		std::vector<std::string> textures;
		READ_DATA(document, textures);
		for(auto texture : textures) {
			// get texture resource
			this->textures.push_back(GET_RESOURCE(Texture, texture));
		}

		READ_DATA(document, color);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind() {
		this->program->Use();
		this->program->SetUniform("material.color", this->color);
		this->program->SetUniform("material.tiling", this->tiling);
		this->program->SetUniform("material.offset", this->offset);

		for(size_t i = 0; i < this->textures.size(); i++) {
			this->textures[i]->SetActive(GL_TEXTURE0 + (int) i);
			this->textures[i]->Bind();
		}
	}

	void Material::ProcessGUI() {
		ImGui::Begin("Material");
		ImGui::ColorEdit4("Color", glm::value_ptr(this->color));
		ImGui::DragFloat2("Tiling", glm::value_ptr(this->tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(this->offset), 0.01f);
		ImGui::End();
	}
}