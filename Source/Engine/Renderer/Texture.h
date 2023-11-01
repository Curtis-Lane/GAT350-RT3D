#pragma once

#include <glad/include/glad/glad.h>
#include <glm/glm/glm.hpp>

#include "Framework/Resource/Resource.h"

namespace nc {
	class Texture : public Resource {
		public:
			Texture() = default;
			~Texture();

			virtual bool Create(std::string filename, ...) override;

			bool Load(const std::string& filename, class Renderer& renderer);
			const glm::ivec2& GetSize() const {return this->size;}

			void SetActive(GLuint unit) {glActiveTexture(unit);}
			void Bind() {glBindTexture(this->target, this->texture);}

			friend class Renderer;

		protected:
			GLuint texture = NULL;
			GLenum target = GL_TEXTURE_2D;
			glm::ivec2 size = glm::ivec2(0);
	};
}
