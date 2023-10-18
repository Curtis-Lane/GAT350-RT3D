#pragma once

#include "Framework/World.h"

#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"

namespace nc {
	class World04 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			//float time = 0.0f;
			float speed = 2.5f;

			glm::vec3 ambientLightColor = glm::vec3(0.2f);
			glm::vec3 diffuseLightColor = glm::vec3(1);
			glm::vec3 diffuseLightPosition = glm::vec3(0, 8, 0);

			Transform transform;

			res_t<Model> model;
	};
}