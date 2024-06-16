#pragma once

#include <vector>
#include <glad/gl.h>
#include <glm/gtc/quaternion.hpp>


class Mesh 
{
public:
	Mesh();
	void Load(const char *name);
	GLuint GetVAO() const;
	GLsizei GetIndicesCount() const;

private:
	void Center(std::vector<glm::vec3>& vertices) const;
	void Normalize(std::vector<glm::vec3>& vertices) const;
	glm::quat orientation;
	GLuint mVAO;
	GLsizei mCount;
};