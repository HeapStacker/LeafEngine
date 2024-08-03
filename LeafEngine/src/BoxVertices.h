#pragma once

namespace lf {
	void initializeBoxVertices();
	unsigned int& getBoxVao();
	unsigned int& getBoxVerticesCount();

	void initializeTexturedBoxVertices();
	unsigned int& getTexturedBoxVao();
	unsigned int& getTexturedBoxVerticesCount();
}