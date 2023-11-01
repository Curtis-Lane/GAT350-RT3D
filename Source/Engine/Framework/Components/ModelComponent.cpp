#include "ModelComponent.h"

#include "Framework/Resource/ResourceManager.h"

#include "Core/StringUtils.h"
#include "Framework/Actor.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		if(!modelName.empty()) {
			model = std::make_shared<Model>();
			model->Load(modelName);
			//ADD_RESOURCE(modelName, model);
		}

		if(model != nullptr && !materialName.empty()) {
			model->SetMaterial(GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		auto material = model->GetMaterial();
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(this->enableDepth);
		glCullFace(this->cullface);

		model->Draw();
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);

		READ_DATA(value, enableDepth);

		std::string cullfaceValue;
		if(READ_NAME_DATA(value, "cullface", cullfaceValue)) {
			if(StringUtils::IsEqualIgnoreCase(cullfaceValue, "front")) {
				cullface = GL_FRONT;
			} else if(StringUtils::IsEqualIgnoreCase(cullfaceValue, "back")) {
				cullface = GL_BACK;
			} else if(StringUtils::IsEqualIgnoreCase(cullfaceValue, "front_and_back")) {
				cullface = GL_FRONT_AND_BACK;
			}
		}
	}
}
