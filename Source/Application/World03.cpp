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

		return true;
	}

	void World03::Shutdown() {
	}

	void World03::Update(float deltaTime) {
		this->time += deltaTime;

		GLint uniform = glGetUniformLocation(this->program->m_program, "time");
		glUniform1f(uniform, this->time);
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
