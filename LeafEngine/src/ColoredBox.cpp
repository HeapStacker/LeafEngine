#include <vector>
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "ColoredBox.h"
#include "BoxVertices.h"

namespace lf {
	static Shader& coloredShader = Shader::getColoredShader();
	static Shader& outlineShader = Shader::getOutlineShader();

	static unsigned int ColoredBoxId = 0;
	static std::vector<ColoredBox*> boxes;

	void ColoredBox::setNewId() {
		id = ColoredBoxId++;
	}

	void ColoredBox::render() {
		coloredShader.use();
		coloredShader.setVec3("color", color);
		coloredShader.setMat4("model", modelMatrix);
		if (outline) {

			// we draw the object
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			coloredShader.use();
			coloredShader.setMat4("model", modelMatrix);
			glBindVertexArray(getBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, getBoxVerticesCount());


			// we scale the object but only draw the parts which were not drawn before
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			static float scale = 1.03f;
			static glm::mat4 tempModel;
			tempModel = glm::scale(this->modelMatrix, glm::vec3(scale, scale, scale));
			outlineShader.use();
			outlineShader.setMat4("model", tempModel);
			glBindVertexArray(getBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, getBoxVerticesCount());


			// we return everything to default
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
		}
		else {
			glStencilMask(0x00);
			glBindVertexArray(getBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, getBoxVerticesCount());
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
		setNewId();
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