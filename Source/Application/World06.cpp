#include "World06.h"

#include <glm/glm/gtx/color_space.hpp>

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

namespace nc {
	bool World06::Initialize() {
		m_scene = std::make_unique<Scene>();
		m_scene->Load("Scenes/scene_framebuffer.json");
		m_scene->Initialize();

		auto texture = std::make_shared<Texture>();
		texture->CreateTexture(512, 512);
		ADD_RESOURCE(Texture, "fb_texture", texture);

		auto framebuffer = std::make_shared<Framebuffer>();
		framebuffer->CreateFramebuffer(texture);
		ADD_RESOURCE(Framebuffer, "fb", framebuffer);

		auto material = GET_RESOURCE(Material, "materials/framebuffer.mtrl");
		if(material != nullptr) {
			material->albedoTexture = texture;
		}

		return true;
	}

	void World06::Shutdown() {
	}

	void World06::Update(float deltaTime) {
		ENGINE.GetSystem<Gui>()->BeginFrame();

		m_scene->Update(deltaTime);
		m_scene->ProcessGUI();

		ENGINE.GetSystem<Gui>()->EndFrame();
	}

	void World06::Draw(Renderer& renderer) {
		// ** PASS 1 **
		m_scene->GetActorByName("cube")->active = false;

		auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
		renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
		framebuffer->Bind();

		renderer.BeginFrame(glm::vec3(0, 0, 1));
		m_scene->Draw(renderer);

		framebuffer->Unbind();

		// ** PASS 2 **
		m_scene->GetActorByName("cube")->active = true;

		renderer.ResetViewport();
		renderer.BeginFrame();
		m_scene->Draw(renderer);

		//

		// post-render
		ENGINE.GetSystem<Gui>()->Draw();
		renderer.EndFrame();
	}
}
