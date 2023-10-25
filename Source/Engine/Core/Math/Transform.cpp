#include "Transform.h"

#include "Renderer/Gui.h"

namespace nc {
	void Transform::ProcessGUI() {
		ImGui::DragFloat3("Position", glm::value_ptr(this->position), 0.1f);
		ImGui::DragFloat3("Rotation", glm::value_ptr(this->rotation));
		ImGui::DragFloat3("Scale", glm::value_ptr(this->scale), 0.1f);
	}

	void Transform::Read(const json_t& value) {
		READ_DATA(value, position);
		READ_DATA(value, rotation);
		READ_DATA(value, scale);
	}
}
