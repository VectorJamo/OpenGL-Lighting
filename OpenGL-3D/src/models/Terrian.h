#pragma once
#include <vector>
#include "glm.hpp"
#include "Vertex.h"

class Terrian
{
private:
	float m_X, m_Z;
	static constexpr float m_SIZE = 300.0f;
	static constexpr int m_VERTICES_PER_AXIS = 30;
	static constexpr int m_TERRIAN_SUBAREA_SIZE = m_SIZE/m_VERTICES_PER_AXIS;

	std::vector<Vertex> m_TerrianVertices;
	std::vector<unsigned short> m_TerrianIndices;
	unsigned int m_VAO, m_VBO, m_IBO;

public:
	Terrian(int gridX, int gridZ);

	inline unsigned int GetTerrianVAO() { return m_VAO; }
	inline std::vector<unsigned short> GetTerrianIndices() { return m_TerrianIndices; }

	inline static constexpr int Terrian::GetTerrianSize() { return m_SIZE; }

private:
	std::vector<Vertex> GenerateTerrianSubPart(int xPos, int zPos, int yPos);

	void GenerateVertices();
	void CreateOpenGLBuffers();
};

