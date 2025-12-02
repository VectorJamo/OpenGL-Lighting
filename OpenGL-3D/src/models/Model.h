#pragma once

#include <vector>
#include <string>
#include "Vertex.h"

class Model
{
private:
	std::string m_ModelPath;

	std::vector<Vertex> m_ModelVerts;
	std::vector<unsigned short> m_ModelIndices;

	unsigned int m_VAO, m_VBO, m_IBO;

public:
	Model(const std::string& modelPath);
	~Model();
	
	inline unsigned int GetModelVAO() { return m_VAO; }
	inline std::vector<Vertex> GetModelVertices() { return m_ModelVerts; }
	inline std::vector<unsigned short> GetModelIndices() { return m_ModelIndices; }

private:
	void LoadModel();
	void CreateOpenGLBuffers();
};

