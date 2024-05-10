#include "shader.h"

#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>

#include <glad/gl.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	auto vertexShader = LoadFile(vertexPath);
	auto fragShader = LoadFile(fragmentPath);

	auto vertex = Compile(vertexShader.c_str(), GL_VERTEX_SHADER);
	auto fragment = Compile(fragShader.c_str(), GL_FRAGMENT_SHADER);
	programID = Link(vertex, fragment);
}

std::string Shader::LoadFile(const char* path) const {
	if (std::ifstream ifs{ path }; ifs.good()) {
		std::stringstream ss;
		ss << ifs.rdbuf();
		return ss.str();
	}
	else {
		std::cerr << "Error loading shader file\n";
		throw std::system_error(std::error_code(), "Error loading shader file");
	}
}

unsigned int Shader::Compile(const char* shader, int shaderType) const {
	unsigned int shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shader, nullptr);
	glCompileShader(shaderId);

	int success;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cerr << "Error compiling shader file\n" << infoLog << std::endl;
	}
	return shaderId;
}

unsigned int Shader::Link(unsigned int vertexShader, unsigned int fragmentShader) const {
	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cerr << "Error linking shader file.\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}

void Shader::Use() const {
	glUseProgram(programID);
}

void Shader::SetBool(std::string_view name, bool value) const {
	glUniform1i(glGetUniformLocation(programID, name.data()), (int)value);
}

void Shader::SetInt(std::string_view name, int value) const {
	glUniform1i(glGetUniformLocation(programID, name.data()), value);
}

void Shader::SetFloat(std::string_view name, float value) const {
	glUniform1f(glGetUniformLocation(programID, name.data()), value);
}