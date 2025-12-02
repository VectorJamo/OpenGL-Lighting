#include "Model.h"

#include <fstream>
#include <iostream>

#include <GLAD/glad.h>


std::vector<std::string> SplitLine(const std::string& line, char delim);

Model::Model(const std::string& modelPath)
	:m_ModelPath(modelPath)
{
	LoadModel();
	CreateOpenGLBuffers();
}

Model::~Model()
{
}

void Model::LoadModel()
{
	std::ifstream stream(m_ModelPath);
	if (!stream.is_open())
	{
		std::cout << "Failed to read model file." << std::endl;
		std::cout << m_ModelPath << std::endl;
		return;
	}

	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textCoords;

	std::string line;
	while (std::getline(stream, line)) 
	{
		if (line[0] == 'v') 
		{
			if (line[1] == 'n') 
			{
				// Normal
				std::vector<std::string> normalAsString = SplitLine(line, ' ');
				normals.emplace_back(glm::vec3(std::stof(normalAsString[1]), std::stof(normalAsString[2]), std::stof(normalAsString[3])));
			}
			else if (line[1] == 't') {
				// Texture coordinate
				std::vector<std::string> tcAsString = SplitLine(line, ' ');
				textCoords.emplace_back(glm::vec2(std::stof(tcAsString[1]), std::stof(tcAsString[2])));
			}
			else {
				// Position
				std::vector<std::string> positionAsString = SplitLine(line, ' ');
				positions.emplace_back(glm::vec3(std::stof(positionAsString[1]), std::stof(positionAsString[2]), std::stof(positionAsString[3])));
			}
		}
	}
	stream.close();

	// Create the vertices according to the mesh's faces data
	stream = std::ifstream(m_ModelPath);
	int numFaces = 0;
	while (std::getline(stream, line)) 
	{
		if (line[0] == 'f') 
		{
			numFaces++;

			std::vector<std::string> faces = SplitLine(line, ' ');
			faces.erase(faces.begin()); // Remove the first item which is just a 'f' as a string

			for (std::string& face : faces) 
			{
				std::vector<std::string> vertexInfo = SplitLine(face, '/');

				glm::vec3 position = positions[std::stoi(vertexInfo[0]) - 1];
				glm::vec3 normal = positions[std::stoi(vertexInfo[1]) - 1];
				glm::vec2 tc = positions[std::stoi(vertexInfo[2]) - 1];

				Vertex vertex(position, tc, normal);
				m_ModelVerts.push_back(vertex);
			}
			m_ModelIndices.push_back((numFaces - 1) * 3 + 0);
			m_ModelIndices.push_back((numFaces - 1) * 3 + 1);
			m_ModelIndices.push_back((numFaces - 1) * 3 + 2);
		}
	}
	stream.close();

	std::cout << "MODEL LOADING COMPLETE!" << std::endl;
	std::cout << "TOTAL VERTICES:" << std::endl;
	std::cout << m_ModelVerts.size() << std::endl;
	std::cout << "TOTAL INDICES:" << std::endl;
	std::cout << m_ModelIndices.size() << std::endl;

}

void Model::CreateOpenGLBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	// Send data to the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_ModelVerts.size(), (void*)&m_ModelVerts[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Send data to the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * m_ModelIndices.size(), (void*)&m_ModelIndices[0], GL_STATIC_DRAW);
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

std::vector<std::string> SplitLine(const std::string& line, char delim)
{
	std::vector<std::string> result;

	std::string token;
	for (int i = 0; i < line.length(); i++) 
	{
		if (line[i] != delim) {
			token += line[i];
		}
		else {
			if (!token.empty()) 
			{
				result.push_back(token);
			}
			token = "";
		}
	}

	if (!token.empty())
		result.push_back(token);

	return result;
}
