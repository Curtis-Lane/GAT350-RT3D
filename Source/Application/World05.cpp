#include "World05.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World05::Initialize() {
		m_scene = std::make_unique<Scene>();

		{
			auto actor = CREATE_CLASS(Actor);
			actor->name = "actor1";
			actor->transform.position = glm::vec3(0, 0, 0);
			auto modelComponent = CREATE_CLASS(ModelComponent);
			modelComponent->model = std::make_shared<Model>();
			modelComponent->model->SetMaterial(GET_RESOURCE(Material, "materials/squirrel.mtrl"));
			modelComponent->model->Load("models/squirrel.glb", glm::vec3(0, -0.7f, 0), glm::vec3(0), glm::vec3(0.4f));
			actor->AddComponent(std::move(modelComponent));
			m_scene->Add(std::move(actor));
		}

		{
			auto actor = CREATE_CLASS(Actor);
			actor->name = "light1";
			actor->transform.position = glm::vec3(3, 3, 3);
			auto lightComponent = CREATE_CLASS(LightComponent);
			lightComponent->type = LightComponent::lightType::Point;
			lightComponent->color = glm::rgbColor(glm::vec3(randomf() * 360, 1, 1));
			lightComponent->intensity = 1;
			lightComponent->range = 20;
			lightComponent->innerAngle = 10.0f;
			lightComponent->outerAngle = 30.0f;
			actor->AddComponent(std::move(lightComponent));
			m_scene->Add(std::move(actor));
		}

		return true;
	}

	void World05::Shutdown() {
	}

	void World05::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(deltaTime);
		//m_scene->ProcessGUI();

		auto actor = m_scene->GetActorByName<Actor>("actor1");

		actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? 90 * -deltaTime : 0;
		actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? 90 * deltaTime : 0;
		actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEUP) ? 90 * -deltaTime : 0;
		actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEDOWN) ? 90 * deltaTime : 0;
		actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? 90 * deltaTime : 0;
		actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? 90 * -deltaTime : 0;

		actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? this->speed * +deltaTime : 0;
		actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? this->speed * -deltaTime : 0;
		actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? this->speed * -deltaTime : 0;
		actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? this->speed * +deltaTime : 0;
		actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? this->speed * +deltaTime : 0;
		actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? this->speed * -deltaTime : 0;

		auto material = actor->GetComponent<ModelComponent>()->model->GetMaterial();
		material->ProcessGUI();
		material->Bind();

		// Lights
		material->GetProgram()->SetUniform("ambientLight", this->ambientLightColor);

		// View
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		material->GetProgram()->SetUniform("view", view);

		// Projection
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
		material->GetProgram()->SetUniform("projection", projection);

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World05::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//this->model->Draw(GL_TRIANGLES);
		m_scene->Draw(renderer);

		ENGINE.GetSystem<Gui>()->Draw();

		// post-render
		renderer.EndFrame();
	}
}
