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

	static float MinimalBoxVertices[] = {
		 // Back face
		 -0.5f, -0.5f, -0.5f,  // Bottom-left
		  0.5f,  0.5f, -0.5f,  // top-right
		  0.5f, -0.5f, -0.5f,  // bottom-right         
		  0.5f,  0.5f, -0.5f,  // top-right
		 -0.5f, -0.5f, -0.5f,  // bottom-left
		 -0.5f,  0.5f, -0.5f,  // top-le ft
		 // Front face		   
		 -0.5f, -0.5f,  0.5f,  // bottom-left
		  0.5f, -0.5f,  0.5f,  // bottom-right
		  0.5f,  0.5f,  0.5f,  // top-right
		  0.5f,  0.5f,  0.5f,  // top-right
		 -0.5f,  0.5f,  0.5f,  // top-left
		 -0.5f, -0.5f,  0.5f,  // bottom-l eft
		 // Left face		   
		 -0.5f,  0.5f,  0.5f,  // top-right
		 -0.5f,  0.5f, -0.5f,  // top-left
		 -0.5f, -0.5f, -0.5f,  // bottom-left
		 -0.5f, -0.5f, -0.5f,  // bottom-left
		 -0.5f, -0.5f,  0.5f,  // bottom-right
		 -0.5f,  0.5f,  0.5f,  // top-rig ht
		 // Right face		   
		  0.5f,  0.5f,  0.5f,  // top-left
		  0.5f, -0.5f, -0.5f,  // bottom-right
		  0.5f,  0.5f, -0.5f,  // top-right         
		  0.5f, -0.5f, -0.5f,  // bottom-right
		  0.5f,  0.5f,  0.5f,  // top-left
		  0.5f, -0.5f,  0.5f,  // bottom-left     
		 // Bottom face		   
		 -0.5f, -0.5f, -0.5f,  // top-right
		  0.5f, -0.5f, -0.5f,  // top-left
		  0.5f, -0.5f,  0.5f,  // bottom-left
		  0.5f, -0.5f,  0.5f,  // bottom-left
		 -0.5f, -0.5f,  0.5f,  // bottom-right
		 -0.5f, -0.5f, -0.5f,  // top-right
		 // Top face			   
		 -0.5f,  0.5f, -0.5f,  // top-left
		  0.5f,  0.5f,  0.5f,  // bottom-right
		  0.5f,  0.5f, -0.5f,  // top-right     
		  0.5f,  0.5f,  0.5f,  // bottom-right
		 -0.5f,  0.5f, -0.5f,  // top-left
		 -0.5f,  0.5f,  0.5f   // bottom-left                      
	};

	static unsigned int MinimalBoxVao, MinimalBoxVbo;

	void initializeMinimalBoxVertices() {
		static bool firstCall = true;
		if (firstCall) {
			glGenVertexArrays(1, &MinimalBoxVao);
			glGenBuffers(1, &MinimalBoxVbo);
			glBindBuffer(GL_ARRAY_BUFFER, MinimalBoxVbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(MinimalBoxVertices), MinimalBoxVertices, GL_STATIC_DRAW); //or GL_DYNAMIC_DRAW for moving objects
			glBindVertexArray(MinimalBoxVao);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			firstCall = false;
		}
	}

	unsigned int&& getMinimalBoxVerticesCount()
	{
		return sizeof(MinimalBoxVertices) / (sizeof(float) * 3);
	}

	unsigned int& getMinimalBoxVao() {
		return MinimalBoxVao;
	}


	// NEXT TEMPLATE -------------------------------------------------------------------------------------------------------------------------------

	static float BoxVertices[] = {		
		// Back face
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	 // Bottom-left
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	 // top-right
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	 // bottom-right         
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	 // top-right
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	 // bottom-left
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,	 // top-left
		// Front face								 
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 // bottom-left
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 // bottom-right
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 // top-right
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 // top-right
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 // top-left
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,	 // bottom-left
		// Left face								 
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // top-right
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // top-left
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f, // top-right
		// Right face								 
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,	 // top-left
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,	 // bottom-right
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,	 // top-right         
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,	 // bottom-right
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,	 // top-left
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,	 // bottom-left     
		 // Bottom face								 
		 -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,	 // top-right
		  0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,	 // top-left
		  0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,	 // bottom-left
		  0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,	 // bottom-left
		 -0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,	 // bottom-right
		 -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,	 // top-right
		 // Top face									 
		 -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,	 // top-left
		  0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,	 // bottom-right
		  0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,	 // top-right     
		  0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,	 // bottom-right
		 -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,	 // top-left
		 -0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f	 // bottom-left                      
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
		// Back face
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 0.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, // top-right
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f,  1.0f,		0.0f, 0.0f, // bottom-left
		// Left face
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f, // top-right
		// Right face
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f, // top-left
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 1.0f, // bottom-right
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		1.0f, 0.0f, // top-left
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,		0.0f, 0.0f, // bottom-left     
		 // Bottom face
		 -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f, // top-right
		  0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 1.0f, // top-left
		  0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f, // bottom-left
		  0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		1.0f, 0.0f, // bottom-left
		 -0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 0.0f, // bottom-right
		 -0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,		0.0f, 1.0f, // top-right
		 // Top face
		 -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f, // top-left
		  0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f, // bottom-right
		  0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 1.0f, // top-right     
		  0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		1.0f, 0.0f, // bottom-right
		 -0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 1.0f, // top-left
		 -0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,		0.0f, 0.0f  // bottom-left   
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