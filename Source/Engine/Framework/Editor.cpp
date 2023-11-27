#include "Editor.h"

#include "Components/CameraComponent.h"
#include "Scene.h"

namespace nc {
	void Editor::Update() {
		if(ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) {
			this->active = !(this->active);
		}
	}

	void Editor::ProcessGUI(Scene* scene) {
		if(!(this->active)) {
			return;
		}

		// Show resources
		ImGui::Begin("Resources");

		// Textures
		if(ImGui::CollapsingHeader("Textures")) {
			auto resources = GET_RESOURCES(Texture);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		// Models
		if(ImGui::CollapsingHeader("Models")) {
			auto resources = GET_RESOURCES(Model);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		// Materials
		if(ImGui::CollapsingHeader("Materials")) {
			auto resources = GET_RESOURCES(Material);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		// Shaders
		if(ImGui::CollapsingHeader("Shaders")) {
			auto resources = GET_RESOURCES(Shader);
			for(auto& resource : resources) {
				if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
					this->selectedObject = resource.get();
				}
			}
		}

		ImGui::End();

		// Show scene
		ImGui::Begin("Scene");
		scene->ProcessGUI();
		ImGui::Separator();

		// Show actors
		ImGui::BeginChild("Actors");
		// Actor controls
		if(ImGui::BeginPopupContextWindow()) {
			auto cameras = scene->GetComponents<CameraComponent>();
			auto camera = (!cameras.empty()) ? cameras[0] : nullptr;

			if(ImGui::MenuItem("Create Empty")) {
				auto actor = CREATE_CLASS(Actor);
				actor->name = StringUtils::CreateUnique(actor->GetClassName());
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Sphere")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Sphere");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Cube")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Cube");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Camera")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Camera");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}
			if(ImGui::MenuItem("Create Light")) {
				auto actor = CREATE_CLASS_BASE(Actor, "Light");
				actor->name = StringUtils::CreateUnique(actor->name);
				if(camera != nullptr) {
					actor->transform.position = camera->m_owner->transform.position + (camera->m_owner->transform.Forward() * 3.0f);
				}
				actor->Initialize();

				this->selectedObject = actor.get();
				scene->Add(std::move(actor));
			}

			ImGui::EndPopup();
		}

		for(auto& actor : scene->m_actors) {
			if(ImGui::Selectable(actor->name.c_str(), actor.get() == this->selectedObject)) {
				this->selectedObject = actor.get();
			}
		}
		ImGui::EndChild();
		ImGui::End();

		// Show inspector
		ImGui::Begin("Inspector");
		if(this->selectedObject != nullptr) {
			this->selectedObject->ProcessGUI();
			// Delete selected actor
			if(ImGui::IsKeyPressed(ImGuiKey_Delete)) {
				auto actor = dynamic_cast<Actor*>(this->selectedObject);
				if(actor != nullptr) {
					scene->Remove(actor);
					this->selectedObject = nullptr;
				}
			}
		}

		ImGui::End();
	}
}