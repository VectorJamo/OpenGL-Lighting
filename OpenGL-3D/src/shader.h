#pragma once
#include <string>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

class Shader
{
private:
	unsigned int m_ShaderProgramID;
	const char* m_VertexShaderPath, *m_FragmentShaderPath;

public:
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	~Shader();

	void Use();
	void SetUniformVec4f(const char* uniformName, float x, float y, float z, float w);

	inline unsigned int GetShaderProgram() { return m_ShaderProgramID; }
private:
	std::string LoadShader(const char* filePath);
	unsigned int CompileShader(int shaderType, const char* shaderCode);
};

