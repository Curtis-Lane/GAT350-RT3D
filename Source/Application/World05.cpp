#include "World05.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World05::Initialize() {
		m_scene = std::make_unique<Scene>();
		m_scene->Load("Scenes/scene.json");
		m_scene->Initialize();

		return true;
	}

	void World05::Shutdown() {
	}

	void World05::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(deltaTime);
		m_scene->ProcessGUI();

		auto actor = m_scene->GetActorByName<Actor>("actor1");

		auto material = actor->GetComponent<ModelComponent>()->material;
		material->ProcessGUI();
		material->Bind();

		material = GET_RESOURCE(Material, "Materials/refraction.mtrl");
		if(material != nullptr) {
			ImGui::Begin("Refraction");

			ImGui::DragFloat("IOR", &(this->refraction), 0.01f, 1, 3);
			auto program = material->GetProgram();
			program->Use();
			program->SetUniform("ior", this->refraction);

			ImGui::End();
		}

		// Lights
		material->GetProgram()->SetUniform("ambientLight", this->ambientLightColor);

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World05::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		m_scene->Draw(renderer);

		ENGINE.GetSystem<Gui>()->Draw();

		// post-render
		renderer.EndFrame();
	}
}
