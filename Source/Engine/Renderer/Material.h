#pragma once

#include "Framework/Resource/Resource.h"

#include <glm/glm/glm.hpp>
#include <vector>
#include <memory>

#include "GUI.h"

namespace nc {
	class Texture;
	class Program;

	class Material : public Resource {
		public:
			virtual bool Create(std::string filename, ...) override;

			void Bind();

			res_t<Program> GetProgram() {return this->program;}
			void ProcessGUI();

		public:
			glm::vec3 diffuse = glm::vec3(1);
			glm::vec3 specular = glm::vec3(1);
			float shininess = 2.0f;

			glm::vec2 tiling = glm::vec2(1);
			glm::vec2 offset = glm::vec2(0);

		private:
			res_t<Program> program;
			std::vector<res_t<Texture>> textures;
	};
}