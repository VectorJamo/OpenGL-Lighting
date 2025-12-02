#include "Vertex.h"

Vertex::Vertex()
{
}

Vertex::Vertex(const glm::vec3& position, const glm::vec2& textCoord, const glm::vec3& normal)
	:position(position), textCoord(textCoord), normal(normal)
{
}
