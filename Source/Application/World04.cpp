#include "World04.h"

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World04::Initialize() {
		auto material = GET_RESOURCE(Material, "Materials/grid.mtrl");
		this->model = std::make_shared<Model>();
		this->model->SetMaterial(material);
		this->model->Load("Models/plane.obj", glm::vec3(0, -1, 0));
		//this->model->Load("Models/plane.obj", glm::vec3(0), glm::vec3(90, 0, 0));
		//this->model->Load("Models/buddha.obj", glm::vec3(0), glm::vec3(-90, 0, 0));
		//this->model->Load("Models/dragon.obj");

		this->light.type = light_t::lightType::Point;
		this->light.position = glm::vec3(0, 5, 0);
		this->light.direction = glm::vec3(0, -1, 0);
		this->light.color = glm::vec3(1);
		this->light.cutoff = 30.0f;

		return true;
	}

	void World04::Shutdown() {
	}

	void World04::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		ImGui::Begin("Transform");
		ImGui::DragFloat3("Position", &this->transform.position[0], 0.1f);
		ImGui::DragFloat3("Rotation", &this->transform.rotation[0]);
		ImGui::DragFloat3("Scale", &this->transform.scale[0], 0.1f);
		ImGui::End();

		this->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? 90 * -deltaTime : 0;
		this->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? 90 * deltaTime : 0;
		this->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEUP) ? 90 * -deltaTime : 0;
		this->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_PAGEDOWN) ? 90 * deltaTime : 0;
		this->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? 90 * deltaTime : 0;
		this->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? 90 * -deltaTime : 0;

		this->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? this->speed * +deltaTime : 0;
		this->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? this->speed * -deltaTime : 0;
		this->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? this->speed * -deltaTime : 0;
		this->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? this->speed * +deltaTime : 0;
		this->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? this->speed * +deltaTime : 0;
		this->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? this->speed * -deltaTime : 0;

		//this->time += deltaTime;

		auto material = this->model->GetMaterial();
		material->ProcessGUI();
		material->Bind();

		ImGui::Begin("Light");
		const char* types[] = {"Point", "Directional", "Spot"};
		ImGui::Combo("Type", (int*) &(this->light.type), types, 3);

		ImGui::ColorEdit3("Ambient Light Color", glm::value_ptr(this->ambientLightColor));

		ImGui::ColorEdit3("Diffuse Light Color", glm::value_ptr(this->light.color));

		if(this->light.type != light_t::lightType::Point) {
			ImGui::DragFloat3("Diffuse Light Direction", glm::value_ptr(this->light.direction));
		}

		if(this->light.type != light_t::lightType::Directional) {
			ImGui::DragFloat3("Diffuse Light Position", glm::value_ptr(this->light.position), 0.25f);
		}

		if(this->light.type == light_t::lightType::Spot) {
			ImGui::DragFloat("Cutoff", &(this->light.cutoff), 1, 0, 90);
		}

		ImGui::End();

		// Lights
		material->GetProgram()->SetUniform("ambientLight", this->ambientLightColor);
		material->GetProgram()->SetUniform("light.color", this->light.color);
		material->GetProgram()->SetUniform("light.position", this->light.position);
		material->GetProgram()->SetUniform("light.direction", this->light.direction);
		material->GetProgram()->SetUniform("light.type", this->light.type);
		material->GetProgram()->SetUniform("light.cutoff", glm::radians(this->light.cutoff));

		// Model
		material->GetProgram()->SetUniform("model", this->transform.GetMatrix());

		// View
		glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		material->GetProgram()->SetUniform("view", view);

		// Projection
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(ENGINE.GetSystem<Renderer>()->GetWidth()) / ENGINE.GetSystem<Renderer>()->GetHeight(), 0.01f, 100.0f);
		material->GetProgram()->SetUniform("projection", projection);

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World04::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->model->Draw(GL_TRIANGLES);

		ENGINE.GetSystem<Gui>()->Draw();

		// post-render
		renderer.EndFrame();
	}
}
