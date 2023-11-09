#pragma once

#include "Framework/World.h"

#include "Core/Math/Transform.h"
#include "Renderer/Renderer.h"

namespace nc {
	class World06 : public World {
		public:
			const uint32_t INVERT_MASK    = 1;
			const uint32_t GRAYSCALE_MASK = 2;
			const uint32_t COLORTINT_MASK = 4;

		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			uint32_t params = 0;
			glm::vec3 colorTint = glm::vec3(0);

			float blend = 1.0f;

			glm::vec3 ambientLightColor = glm::vec3(0.2f);
	};
}