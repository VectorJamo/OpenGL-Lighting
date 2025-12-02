#pragma once

#include "glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textCoord;
	glm::vec3 normal;

	Vertex();
	Vertex(const glm::vec3& position, const glm::vec2& textCoord, const glm::vec3& normal);
};

