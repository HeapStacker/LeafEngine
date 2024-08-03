#include <vector>
#include "Window.h"
#include "Camera.h"
#include <stb_image.h>
#include <glm/glm.hpp>
#include "TexturedBox.h"
#include "ModelLoader.h"
#include "BoxVertices.h"


namespace lf {
	static Shader& normalShader = Shader::getNormalShader();
	static Shader& outlineShader = Shader::getOutlineShader();

	static unsigned int TexturedBoxId = 0;
	static std::vector<TexturedBox*> boxes;

	void TexturedBox::setNewId() {
		id = TexturedBoxId++;
	}

	void TexturedBox::render() {
		normalShader.use();
		normalShader.setMat4("model", modelMatrix);
		if (outline) {
			// we draw the object
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			normalShader.use();
			normalShader.setMat4("model", modelMatrix);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
			glBindVertexArray(getTexturedBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, getTexturedBoxVerticesCount());


			// we scale the object but only draw the parts which were not drawn before
			static float scale = 1.03f;
			static glm::mat4 tempModel;
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
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
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);
			glBindVertexArray(getTexturedBoxVao());
			glDrawArrays(GL_TRIANGLES, 0, getTexturedBoxVerticesCount());
		}
	}

	void TexturedBox::RenderTexturedBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
		normalShader.use();
		normalShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		normalShader.setMat4("projection", projectionMatrix);
		normalShader.setMat4("view", viewMatrix);
		outlineShader.use();
		outlineShader.setMat4("projection", projectionMatrix);
		outlineShader.setMat4("view", viewMatrix);
		for (TexturedBox* box : boxes) {
			if (box->visible) {
				box->render();
			}
		}
	}

	TexturedBox::TexturedBox(const glm::vec3& position, const char* diffuseTexturePath, const char* specularTexturePath) {
		setNewId();
		initializeTexturedBoxVertices();
		setPosition(position);
		diffuseMap = internal::TextureFromFile(diffuseTexturePath, ".");
		specularMap = internal::TextureFromFile(specularTexturePath, ".");
		boxes.push_back(this);
	}

	void TexturedBox::setVisibility(bool visible) {
		this->visible = visible;
	}

	void TexturedBox::setOutline(bool outline) {
		this->outline = outline;
	}

	void TexturedBox::remove() {
		boxes.erase(std::remove(boxes.begin(), boxes.end(), this), boxes.end());
	}
}
