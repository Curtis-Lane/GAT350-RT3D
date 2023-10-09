#include "World03.h"

#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#define INTERLEAVE

namespace nc {
	bool World03::Initialize() {
		// Shaders
		this->program = GET_RESOURCE(Program, "Shaders/unlit_color.prog");
		this->program->Use();

		#ifdef INTERLEAVE
		
		// Vertex data
		float vertexData[] = {
			-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f,
			-0.8f,  0.8f, 0.0f, 0.0f, 1.0f, 1.0f,
			-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,
			 0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.8f,  0.8f, 0.0f, 1.0f, 1.0f, 0.0f
		};

		GLuint vbo;
		glGenBuffers(1, &vbo);
		// Position
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);

		glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat) * 6);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexAttribBinding(0, 0);

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
		glVertexAttribBinding(1, 0);

		#else

		// Vertex data
		float positionData[] = {
			-0.8f, -0.8f, 0.0f,
			 0.8f,  0.8f, 0.0f,
			-0.8f,  0.8f, 0.0f,
			-0.8f, -0.8f, 0.0f,
			 0.8f, -0.8f, 0.0f,
			 0.8f,  0.8f, 0.0f
		};

		float colorData[] =	{
			1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f
		};

		GLuint vbo[2];
		glGenBuffers(2, vbo);
		// Position
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

		// Color
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

		glGenVertexArrays(1, &(this->vao));
		glBindVertexArray(this->vao);

		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexBuffer(0, vbo[0], 0, sizeof(GLfloat) * 3);

		// Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindVertexBuffer(1, vbo[1], 0, sizeof(GLfloat) * 3);

		#endif

		//this->position.z = -5.0f;

		return true;
	}

	void World03::Shutdown() {
	}

	void World03::Update(float deltaTime) {
		this->angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? 90 * deltaTime : 0;
		this->angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? 90 * -deltaTime : 0;

		this->position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? this->speed * +deltaTime : 0;
		this->position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? this->speed * -deltaTime : 0;
		this->position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? this->speed * +deltaTime : 0;
		this->position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? this->speed * -deltaTime : 0;
		this->position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? this->speed * -deltaTime : 0;
		this->position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? this->speed * +deltaTime : 0;

		this->time += deltaTime;

		// Model
		glm::mat4 position = glm::translate(glm::mat4(1), this->position);
		glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(this->angle), glm::vec3(0, 0, 1));

		glm::mat4 model = position * rotation;
		GLint uniform = glGetUniformLocation(this->program->m_program, "model");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(model));

		// View
		glm::mat4 view = glm::lookAt(glm::vec3(0, 4, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		uniform = glGetUniformLocation(this->program->m_program, "view");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(view));

		// Projection
		glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
		uniform = glGetUniformLocation(this->program->m_program, "projection");
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(projection));
	}

	void World03::Draw(Renderer& renderer) {
		// pre-render
		renderer.BeginFrame();

		// render
		glBindVertexArray(this->vao);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// post-render
		renderer.EndFrame();
	}
}
