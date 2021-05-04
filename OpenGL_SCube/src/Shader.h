#pragma once
#include <iostream>
#include <unordered_map>
#include <glm.hpp>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};


class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const; 

	//set Uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix );
	void SetUniformVec4f(const std::string& name, const glm::vec4& vec);

	void SetUniformVec3f(const std::string& name, const glm::vec3& vec);

	void SetUniformVec4i(const std::string& name, const glm::ivec4& vec);

	
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name) const;
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	
};