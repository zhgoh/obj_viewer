#include "mesh.h"

#include <iostream>
#include <fstream>
#include <exception>
#include <string>
#include <vector>

static
std::vector<std::string_view> tokenize(std::string_view s, std::string_view del = " ") {
	std::vector<std::string_view> res;
	size_t start = 0, end = -1 * del.size();
	do {
		start = end + del.size();
		end = s.find(del, start);

		res.emplace_back(s.substr(start, end - start));
	} while (end != -1);
	return res;
}

static
std::string_view ltrim(std::string_view s) {
	s.remove_prefix(std::distance(s.cbegin(), std::find_if(s.cbegin(), s.cend(),
		[](int c) {return !std::isspace(c); })));

	return s;
}

static
std::string_view rtrim(std::string_view s) {
	s.remove_suffix(std::distance(s.crbegin(), std::find_if(s.crbegin(), s.crend(),
		[](int c) {return !std::isspace(c); })));

	return s;
}

static
std::string_view trim(std::string_view s) {
	return ltrim(rtrim(s));
}

Mesh::Mesh() 
	: orientation{}, mVAO{}, mCount{} {
}

void Mesh::Load(const char* name) {
	// Only load mesh without textures
	std::vector<glm::vec3> vertices;
	std::vector<unsigned int> indices;

	std::ifstream ifs{ name };
	if (ifs.bad()) {
		throw std::runtime_error("Error loading mesh");
	}

	while (!ifs.eof()) {
		std::string line;
		ifs >> line;

		if (line == "v") {
			float arr[3]{};
			ifs >> arr[0] >> arr[1] >> arr[2];
			vertices.emplace_back(glm::vec3{ arr[0], arr[1], arr[2] });
		}
		else if (line == "f") {
			std::getline(ifs, line);

			for (auto v : tokenize(trim(line))) {
				// 1 set of indices/vtx tex coord indices/vtx normal indices
				auto token = tokenize(v, "/");
				const auto id = atoi(token[0].data()) - 1;	// index start with 0
				indices.emplace_back(id);
			}
		}
		else {
			std::getline(ifs, line);
		}
	}

	mCount = indices.size();

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof vertices[0], vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof indices[0], indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

GLuint Mesh::GetVAO() const {
	return mVAO;
}

size_t Mesh::GetIndicesCount() const {
	return mCount;
}