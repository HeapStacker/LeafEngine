#include <glad/glad.h>
#include "Shapes.h"

namespace lf {
	static float RectangleVertices[] = {
		0.0f,  0.5f,  0.0f,		0.0f,  0.0f, -1.0f,		0.0f,  1.0f,
		0.0f, -0.5f,  0.0f,		0.0f,  0.0f, -1.0f,		0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,		0.0f,  0.0f, -1.0f,		1.0f,  0.0f,
		0.0f,  0.5f,  0.0f,		0.0f,  0.0f, -1.0f,		0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,		0.0f,  0.0f, -1.0f,		1.0f,  0.0f,
		1.0f,  0.5f,  0.0f,		0.0f,  0.0f, -1.0f,		1.0f,  1.0f
	};

	static unsigned int RectangleVao, RectangleVbo;

	void initializeRectangleVertices() {
		static bool firstCall = true;
		if (firstCall) {
			glGenVertexArrays(1, &RectangleVao);
			glGenBuffers(1, &RectangleVbo);
			glBindBuffer(GL_ARRAY_BUFFER, RectangleVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(RectangleVertices), RectangleVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
			glBindVertexArray(RectangleVao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			firstCall = false;
		}
	}

	unsigned int&& getRectangleVerticesCount()
	{
		return sizeof(RectangleVertices) / (sizeof(float) * 8);
	}

	unsigned int& getRectangleVao() {
		return RectangleVao;
	}


	// NEXT TEMPLATE -------------------------------------------------------------------------------------------------------------------------------

	static float BoxVertices[] = {		
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

	static unsigned int BoxVao, BoxVbo;

	void initializeBoxVertices() {
		static bool firstCall = true;
		if (firstCall) {
			glGenVertexArrays(1, &BoxVao);
			glGenBuffers(1, &BoxVbo);
			glBindBuffer(GL_ARRAY_BUFFER, BoxVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(BoxVertices), BoxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
			glBindVertexArray(BoxVao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			firstCall = false;
		}
	}

	unsigned int&& getBoxVerticesCount()
	{
		return sizeof(BoxVertices) / (sizeof(float) * 6);
	}

	unsigned int& getBoxVao() {
		return BoxVao;
	}


	// NEXT TEMPLATE -------------------------------------------------------------------------------------------------------------------------------

	static float TexturedBoxVertices[] = {
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

	static unsigned int TexturedBoxVao, TexturedBoxVbo;

	void initializeTexturedBoxVertices() {
		static bool firstCall = true;
		if (firstCall) {
			glGenVertexArrays(1, &TexturedBoxVao);
			glGenBuffers(1, &TexturedBoxVbo);
			glBindBuffer(GL_ARRAY_BUFFER, TexturedBoxVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(TexturedBoxVertices), TexturedBoxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
			glBindVertexArray(TexturedBoxVao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			firstCall = false;
		}
	}

	unsigned int&& getTexturedBoxVerticesCount()
	{
		return sizeof(TexturedBoxVertices) / (sizeof(float) * 8);
	}

	unsigned int& getTexturedBoxVao() {
		return TexturedBoxVao;
	}
}