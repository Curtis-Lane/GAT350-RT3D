#pragma once

#include "Framework/World.h"
#include "Renderer/Renderer.h"

namespace nc {
	class World03 : public World {
		public:
			bool Initialize() override;
			void Shutdown() override;
			void Update(float deltaTime) override;
			void Draw(Renderer& renderer) override;

		private:
			//float time = 0.0f;
			float speed = 5.0f;

			Transform transform;

			GLuint vao = 0;
			res_t<Program> program;
	};
}