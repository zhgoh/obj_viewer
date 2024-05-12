#pragma once

#include <glad/gl.h>
#include <glm/gtc/quaternion.hpp>


class Mesh {
public:
	Mesh();
	void Load(const char *name);
	GLuint GetVAO() const;
	size_t GetIndicesCount() const;

private:
	glm::quat orientation;
	GLuint mVAO;
	size_t mCount;
};