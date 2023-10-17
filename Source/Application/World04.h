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
			float speed = 5.0f;

			Transform transform;

			res_t<Model> model;
	};
}