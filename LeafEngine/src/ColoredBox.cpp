#include <vector>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "ColoredBox.h"
#include "Shapes.h"

namespace lf {
	static Shader& coloredShader = Shader::getColoredShader();
	static Shader& outlineShader = Shader::getOutlineShader();
	static unsigned int BoxVerticesCount = getBoxVerticesCount();

	static unsigned int ColoredBoxId = 0;
	static std::vector<ColoredBox*> boxes;

	static float outlineScale = 1.03f;
	static glm::mat4 tempModel;
	void ColoredBox::render() {
		coloredShader.use();
		coloredShader.setVec3("color", color);
		coloredShader.setMat4("model", this->SpatialObject::modelMatrix);
		if (outline) {
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			coloredShader.use();
			coloredShader.setMat4("model", this->SpatialObject::modelMatrix);
			glBindVertexArray(getBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, BoxVerticesCount);

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			tempModel = glm::scale(this->SpatialObject::modelMatrix, glm::vec3(outlineScale));
			outlineShader.use();
			outlineShader.setMat4("model", tempModel);
			glBindVertexArray(getBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, BoxVerticesCount);

			glStencilFunc(GL_ALWAYS, 0, 0xFF);
		}
		else {
			glBindVertexArray(getBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, BoxVerticesCount);
		}
	}

	void ColoredBox::RenderColoredBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
		coloredShader.use();
		coloredShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		coloredShader.setMat4("projection", projectionMatrix);
		coloredShader.setMat4("view", viewMatrix);
		outlineShader.use();
		outlineShader.setMat4("projection", projectionMatrix);
		outlineShader.setMat4("view", viewMatrix);
		for (ColoredBox* box : boxes) {
			if (box->visible) box->render();
		}
	}

	ColoredBox::ColoredBox(const glm::vec3& position, const glm::vec3& color) {
		initializeBoxVertices();
		setPosition(position);
		this->color = color;
		boxes.push_back(this);
	}

	void ColoredBox::setVisibility(bool visible) {
		this->visible = visible;
	}

	void ColoredBox::setOutline(bool outline) {
		this->outline = outline;
	}

	void ColoredBox::remove() {
		boxes.erase(std::remove(boxes.begin(), boxes.end(), this), boxes.end());
	}

}