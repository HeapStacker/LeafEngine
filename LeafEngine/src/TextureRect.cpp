#include <map>
#include "Shapes.h"
#include "Shader.h"
#include "TextureRect.h"
#include "ModelLoader.h"

namespace lf {
	static Shader& normalShader = Shader::getNormalShader();
	static unsigned int RectangleVerticesCount = getRectangleVerticesCount();

	static unsigned int RectangleId = 0;
	static std::vector<TextureRect*> Rectangles;
	static std::vector<TextureRect*> TransparentRectangles;
	static std::map<float, glm::vec3> sorted;

	void TextureRect::render() {
		normalShader.setMat4("model", this->SpatialObject::modelMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glBindVertexArray(getRectangleVao());
		glDrawArrays(GL_TRIANGLES, 0, RectangleVerticesCount);
	}

	void TextureRect::RenderRects(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
		normalShader.use();
		normalShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		normalShader.setMat4("projection", projectionMatrix);
		normalShader.setMat4("view", viewMatrix);
		for (TextureRect* rect : Rectangles) {
			if (rect->visible) {
				rect->render();
			}
		}
	}

	void TextureRect::RenderTransparentRects(glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
		normalShader.use();
		normalShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		normalShader.setMat4("projection", projectionMatrix);
		normalShader.setMat4("view", viewMatrix);
		std::sort(TransparentRectangles.begin(), TransparentRectangles.end(), [](TextureRect* a, TextureRect* b) { 
			float distanceToA = glm::length(Camera::GetActiveCamera()->Position - a->getPosition());
			float distanceToB = glm::length(Camera::GetActiveCamera()->Position - b->getPosition());
			return distanceToA > distanceToB;
		});
		for (TextureRect* rect : TransparentRectangles) {
			if (rect->visible) {
				rect->render();
			}
		}
	}

	TextureRect::TextureRect(const glm::vec3& position, const char* diffuseTexturePath, const char* specularTexturePath) {
		initializeRectangleVertices();
		setPosition(position);
		diffuseMap = internal::TextureFromFile(diffuseTexturePath, ".", &transparent);
		specularMap = internal::TextureFromFile(specularTexturePath, ".", &transparent);
		if (transparent) {
			TransparentRectangles.push_back(this);
		}
		else {
			Rectangles.push_back(this);
		}
	}
	
	TextureRect::TextureRect(MovableObject* position, const char* diffuseTexturePath, const char* specularTexturePath) {
		initializeRectangleVertices();
		setPosition(position);
		diffuseMap = internal::TextureFromFile(diffuseTexturePath, ".", &transparent);
		specularMap = internal::TextureFromFile(specularTexturePath, ".", &transparent);
		if (transparent) {
			TransparentRectangles.push_back(this);
		}
		else {
			Rectangles.push_back(this);
		}
	}

	void TextureRect::setVisibility(bool visible) {
		this->visible = visible;
	}

	void TextureRect::remove() {
		Rectangles.erase(std::remove(Rectangles.begin(), Rectangles.end(), this), Rectangles.end());
		TransparentRectangles.erase(std::remove(TransparentRectangles.begin(), TransparentRectangles.end(), this), TransparentRectangles.end());
	}
}