#include "TexturedBox.h"
#include <stb_image.h>
#include "ShaderImplementation.h"
#include <glad/glad.h>
#include <vector>
#include "ModelLoader.h"

#include "Window.h"
#include "Camera.h"
#include <glm/glm.hpp>

namespace lf {
	static Shader& normalShader = getNormalShader();

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
		setShaderDrawProperties(&normalShader, Camera::GetActiveCamera(), viewMatrix, projectionMatrix);
		for (int i = 0; i < texturedBoxes.size(); i++) {
			texturedBoxes[i]->render();
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
}
