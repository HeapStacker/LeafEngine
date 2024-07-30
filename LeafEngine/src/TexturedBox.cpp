#include <vector>
#include "Window.h"
#include "Camera.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include "TexturedBox.h"
#include "ModelLoader.h"


namespace lf {
	static Shader& normalShader = Shader::getNormalShader();

	static float texturedBoxVertices[] = {	
		// positions			 // normals			        // texture coords
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
								 					     
		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
								 					     
		-0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
								 					     
		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	 1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	 1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
								 					     
		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	 0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	 0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
								 					     
		-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	 0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	 0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	static unsigned int TexturedBoxId = 0;
	static std::vector<TexturedBox*> texturedBoxes;

	static unsigned int texturedBoxVao, texturedBoxVbo;
	static unsigned int texturedBoxVerticesCount = 0;

	static void initializeTexturedBox() {
		static bool firstCall = true;
		if (firstCall) {
			glGenVertexArrays(1, &texturedBoxVao);
			glGenBuffers(1, &texturedBoxVbo);
			glBindBuffer(GL_ARRAY_BUFFER, texturedBoxVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texturedBoxVertices), texturedBoxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
			glBindVertexArray(texturedBoxVao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			texturedBoxVerticesCount = sizeof(texturedBoxVertices) / sizeof(float);
			firstCall = false;
		}
	}

	void TexturedBox::setNewId()
	{
		id = TexturedBoxId++;
	}

	void TexturedBox::render()
	{
		normalShader.setMat4("model", modelMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glBindVertexArray(texturedBoxVao);
		glDrawArrays(GL_TRIANGLES, 0, texturedBoxVerticesCount);
	}

	void TexturedBox::RenderTexturedBoxes(glm::mat4& viewMatrix, glm::mat4& projectionMatrix)
	{
		normalShader.use();
		normalShader.setVec3("viewPos", Camera::GetActiveCamera()->Position);
		normalShader.setMat4("projection", projectionMatrix);
		normalShader.setMat4("view", viewMatrix);
		for (TexturedBox* box : texturedBoxes) {
			if (box->visible) {
				box->render();
			}
		}
	}

	TexturedBox::TexturedBox(const char* diffuseTexturePath, const char* specularTexturePath)
	{
		setNewId();
		initializeTexturedBox();
		diffuseMap = internal::TextureFromFile(diffuseTexturePath, ".");
		specularMap = internal::TextureFromFile(specularTexturePath, ".");
		texturedBoxes.push_back(this);
	}

	void TexturedBox::setVisibility(bool visible)
	{
		this->visible = visible;
	}

	void TexturedBox::remove()
	{
		texturedBoxes.erase(std::remove(texturedBoxes.begin(), texturedBoxes.end(), this), texturedBoxes.end());
	}
}
