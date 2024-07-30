#include <vector>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include <glad/glad.h>
#include "ColoredBox.h"

namespace lf {
	static Shader& coloredShader = Shader::getColoredShader();

	static float coloredBoxVertices[] = {
		// positions             // normals			
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f
	};

	static unsigned int ColoredBoxId = 0;
	static std::vector<ColoredBox*> coloredBoxes;

	void ColoredBox::setNewId()
	{
		id = ColoredBoxId++;
	}

	static unsigned int coloredBoxVao, coloredBoxVbo;
	static unsigned int coloredBoxVerticesCount = 0;

	static void initializeColoredBox() {
		static bool firstCall = true;
		if (firstCall) {
			glGenVertexArrays(1, &coloredBoxVao);
			glGenBuffers(1, &coloredBoxVbo);
			glBindBuffer(GL_ARRAY_BUFFER, coloredBoxVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(coloredBoxVertices), coloredBoxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
			glBindVertexArray(coloredBoxVao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			coloredBoxVerticesCount = sizeof(coloredBoxVertices) / sizeof(float);
			firstCall = false;
		}
	}

	void ColoredBox::RenderColoredBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		coloredShader.use();
		coloredShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		coloredShader.setMat4("projection", projectionMatrix);
		coloredShader.setMat4("view", viewMatrix);
		for (ColoredBox* box : coloredBoxes) box->render();
	}

	ColoredBox::ColoredBox(const glm::vec3& color)
	{
		setNewId();
		initializeColoredBox();
		this->color = color;
		coloredBoxes.push_back(this);
	}

	void ColoredBox::setVisibility(bool visible)
	{
		this->visible = visible;
	}

	void ColoredBox::render() {
		coloredShader.setVec3("color", color);
		coloredShader.setMat4("model", modelMatrix);
		glBindVertexArray(coloredBoxVao);
		glDrawArrays(GL_TRIANGLES, 0, coloredBoxVerticesCount);
	}
}