#pragma once

#include <glad/gl.h>
#include <glm/gtc/quaternion.hpp>


class Mesh {
public:
	Mesh();
	void Load(const char *name);
	GLuint GetVAO() const;
	GLsizei GetIndicesCount() const;

private:
	glm::quat orientation;
	GLuint mVAO;
	GLsizei mCount;
};