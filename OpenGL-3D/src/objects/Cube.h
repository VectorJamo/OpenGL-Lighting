#pragma once
#include "glm.hpp"

struct Cube
{
	static constexpr int vertexSize = 8;
	static constexpr int totalVertices = 36;

	float vertices[vertexSize * totalVertices];

	Cube();
};