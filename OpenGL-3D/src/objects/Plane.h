#pragma once
#include "glm.hpp"

class Plane
{
	static constexpr int vertexSize = 6;
	static constexpr int totalVertices = 36;

	float vertices[vertexSize * totalVertices];

	Plane();
};

