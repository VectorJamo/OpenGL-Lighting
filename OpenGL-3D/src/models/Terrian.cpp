#include "Terrian.h"

#include <iostream>
#include <GLAD/glad.h>

Terrian::Terrian(int gridX, int gridZ)
{
	m_X = gridX * m_SIZE;
	m_Z = gridZ * m_SIZE;

	GenerateVertices();
	CreateOpenGLBuffers();
}

std::vector<Vertex> Terrian::GenerateTerrianSubPart(int xPos, int zPos, int yPos)
{
	std::vector<Vertex> subTerrian;
	subTerrian.emplace_back(Vertex(glm::vec3(xPos, yPos, zPos), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	subTerrian.emplace_back(Vertex(glm::vec3(xPos + m_TERRIAN_SUBAREA_SIZE, yPos, zPos), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	subTerrian.emplace_back(Vertex(glm::vec3(xPos + m_TERRIAN_SUBAREA_SIZE, yPos, zPos - m_TERRIAN_SUBAREA_SIZE), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	subTerrian.emplace_back(Vertex(glm::vec3(xPos, yPos, zPos - m_TERRIAN_SUBAREA_SIZE), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

	// Add the indices
	int currentSubTerrians = m_TerrianVertices.size() / 4;

	m_TerrianIndices.push_back(currentSubTerrians*4 + 0);
	m_TerrianIndices.push_back(currentSubTerrians*4 + 1);
	m_TerrianIndices.push_back(currentSubTerrians*4 + 2);
	m_TerrianIndices.push_back(currentSubTerrians*4 + 0);
	m_TerrianIndices.push_back(currentSubTerrians*4 + 2);
	m_TerrianIndices.push_back(currentSubTerrians*4 + 3);

	return subTerrian;
}

void Terrian::GenerateVertices()
{
	int startX = m_X;
	int startZ = m_Z;

	for (int i = 0; i < m_VERTICES_PER_AXIS; i++) 
	{
		for (int j = 0; j < m_VERTICES_PER_AXIS; j++)
		{
			int xPos = startX + j * m_TERRIAN_SUBAREA_SIZE;
			int zPos = startZ - i * m_TERRIAN_SUBAREA_SIZE;

			std::vector<Vertex> subTerrianVerts = GenerateTerrianSubPart(xPos, zPos, 0.0f);
			for (auto& vert : subTerrianVerts)
			{
				m_TerrianVertices.push_back(vert);
			}
		}
	}

	std::cout << "GENRATED TERRIAN VERTS!" << std::endl;
	std::cout << m_TerrianVertices.size() << std::endl;
	std::cout << "GENRATED TERRIAN INDICES!" << std::endl;
	std::cout << m_TerrianIndices.size() << std::endl;
}

void Terrian::CreateOpenGLBuffers()
{
	glGenVertexArrays(1, &m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	// Send data to the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_TerrianVertices.size(), (void*)&m_TerrianVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Send data to the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * m_TerrianIndices.size(), (void*)&m_TerrianIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

