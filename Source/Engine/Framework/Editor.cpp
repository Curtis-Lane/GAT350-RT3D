#include "Editor.h"

#include "Scene.h"

namespace nc {
	void Editor::ProcessGUI(Scene* scene) {
		// Show resources
		ImGui::Begin("Resources");
		auto resources = GET_RESOURCES(Resource);
		for(auto& resource : resources) {
			if(ImGui::Selectable(resource->name.c_str(), resource.get() == this->selectedObject)) {
				this->selectedObject = resource.get();
			}
		}
		ImGui::End();

		// Show scene
		ImGui::Begin("Scene");
		scene->ProcessGUI();
		ImGui::Separator();

		// Show actors
		for(auto& actor : scene->m_actors) {
			if(ImGui::Selectable(actor->name.c_str(), actor.get() == this->selectedObject)) {
				this->selectedObject = actor.get();
			}
		}
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