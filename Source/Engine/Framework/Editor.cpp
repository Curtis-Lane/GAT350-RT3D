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

		// Set seperator widget to yellow to stand out
		ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(1, 1, 0, 1));

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
		int index = 0;
		for(auto& actor : scene->m_actors) {
			bool done = false;
			ImGui::PushID(index++);

			// Rename actor if renameObject is set to an actor
			if(actor.get() == this->renameObject) {
				// Set the keyboard to enter input to the InputText
				if(ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
					ImGui::SetKeyboardFocusHere(0);
				}

				// Create name from actor name, name string is mutable (can be changed)
				char name[32];
				strcpy_s(name, 32, actor->name.c_str());

				// Get input and set new name after return is pressed, clear renameObject to nullptr when done
				if(ImGui::InputText("", name, 32, ImGuiInputTextFlags_EnterReturnsTrue)) {
					actor->name = name;
					this->renameObject = nullptr;
				}
			} else if(ImGui::Selectable(actor->name.c_str(), actor.get() == this->selectedObject)) {
				this->selectedObject = actor.get();
			}

			// Set rename when selected actor is double-clicked
			if(actor.get() == this->selectedObject && ImGui::IsMouseDoubleClicked(0)) {
				this->renameObject = actor.get();
			}

			// Show popup menu when actor is right clicked
			if(ImGui::BeginPopupContextItem(actor->name.c_str(), ImGuiPopupFlags_MouseButtonRight)) {
				if(ImGui::MenuItem("Duplicate")) {
					// Get the raw pointer of the clone of the actor
					auto newActor = dynamic_cast<Actor*>(actor->Clone().release());

					// Check if there is a trailing number on the actor being cloned
					int number = StringUtils::GetTrailingNumber(actor->name);

					// Increment the number if a number existed else add "1" to the name
					newActor->name = (number != -1) ? StringUtils::RemoveTrailingNumber(actor->name) + std::to_string(++number) : actor->name + "1";
					this->selectedObject = newActor;

					// Add new clone after the actor cloned
					scene->Add(std::unique_ptr<Actor>(newActor), actor.get());

					done = true;
				}
				if(ImGui::MenuItem("Delete")) {
					// Remove actor
					scene->Remove(actor.get());
					this->selectedObject = nullptr;
					done = true;
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();

			// If done with right-click operation, exit showing actors
			if(done) {
				break;
			}
		}

		// Actor creation
		ImGui::BeginChild("ActorCreate");
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

		// Need to pop the style at the end
		ImGui::PopStyleColor();

		ImGui::End();
	}
}