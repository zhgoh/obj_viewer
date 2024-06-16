#pragma once

#include <string>
#include <string_view>
#include <glm/matrix.hpp>

class Shader 
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void Use() const;
	void SetBool(const std::string_view name, bool value) const;
	void SetInt(const std::string_view name, int value) const;
	void SetFloat(const std::string_view name, float value) const;
	void SetMatrix(std::string_view name, glm::mat4 value) const;

private:
	std::string LoadFile(const char* path) const;
	unsigned int Compile(const char* shader, int shaderType) const;
	unsigned int Link(unsigned int vertexShader, unsigned int fragmentShader) const;

	unsigned int programID;
};