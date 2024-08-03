#pragma once

namespace lf {
	void initializeRectangleVertices();
	unsigned int&& getRectangleVerticesCount();
	unsigned int& getRectangleVao();

	void initializeBoxVertices();
	unsigned int&& getBoxVerticesCount();
	unsigned int& getBoxVao();

	void initializeTexturedBoxVertices();
	unsigned int&& getTexturedBoxVerticesCount();
	unsigned int& getTexturedBoxVao();
}