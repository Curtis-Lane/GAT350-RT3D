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

		// read the textures
		std::string albedoTextureName;
		READ_NAME_DATA(document, "albedoTexture", albedoTextureName);
		if(!albedoTextureName.empty()) {
			params |= ALBEDO_TEXTURE_MASK;
			this->albedoTexture = GET_RESOURCE(Texture, albedoTextureName);
		}
		std::string specularTextureName;
		READ_NAME_DATA(document, "specularTexture", specularTextureName);
		if(!specularTextureName.empty()) {
			params |= SPECULAR_TEXTURE_MASK;
			this->specularTexture = GET_RESOURCE(Texture, specularTextureName);
		}
		std::string emissiveTextureName;
		READ_NAME_DATA(document, "emissiveTexture", emissiveTextureName);
		if(!emissiveTextureName.empty()) {
			params |= EMISSIVE_TEXTURE_MASK;
			this->emissiveTexture = GET_RESOURCE(Texture, emissiveTextureName);
		}
		std::string normalTextureName;
		READ_NAME_DATA(document, "normalTexture", normalTextureName);
		if(!normalTextureName.empty()) {
			params |= NORMAL_TEXTURE_MASK;
			this->normalTexture = GET_RESOURCE(Texture, normalTextureName);
		}

		READ_DATA(document, albedo);
		READ_DATA(document, specular);
		READ_DATA(document, emissive);
		READ_DATA(document, shininess);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind() {
		this->program->Use();
		this->program->SetUniform("material.params", this->params);

		this->program->SetUniform("material.albedo", this->albedo);
		this->program->SetUniform("material.specular", this->specular);
		this->program->SetUniform("material.emissive", this->emissive);
		this->program->SetUniform("material.shininess", this->shininess);

		this->program->SetUniform("material.tiling", this->tiling);
		this->program->SetUniform("material.offset", this->offset);

		if(albedoTexture != nullptr) {
			albedoTexture->SetActive(GL_TEXTURE0);
			albedoTexture->Bind();
		}
		if(specularTexture != nullptr) {
			specularTexture->SetActive(GL_TEXTURE1);
			specularTexture->Bind();
		}
		if(normalTexture != nullptr) {
			normalTexture->SetActive(GL_TEXTURE2);
			normalTexture->Bind();
		}
		if(emissiveTexture != nullptr) {
			emissiveTexture->SetActive(GL_TEXTURE3);
			emissiveTexture->Bind();
		}
	}

	void Material::ProcessGUI() {
		ImGui::Begin("Material");
		ImGui::ColorEdit3("Albedo", glm::value_ptr(this->albedo));
		ImGui::ColorEdit3("Specular", glm::value_ptr(this->specular));
		ImGui::ColorEdit3("Emissive", glm::value_ptr(this->emissive));
		ImGui::DragFloat("Shininess", &(this->shininess), 0.1f, 2.0f, 216.0f);
		ImGui::DragFloat2("Tiling", glm::value_ptr(this->tiling), 0.1f);
		ImGui::DragFloat2("Offset", glm::value_ptr(this->offset), 0.01f);
		ImGui::End();
	}
}