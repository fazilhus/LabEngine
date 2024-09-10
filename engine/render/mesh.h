#pragma once

#include "gl/glew.h"

#include <vector>

class Mesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

public:
	Mesh();
	~Mesh() = default;

	void Init(GLfloat* vb, GLuint* ib, const std::size_t* sizes, const std::size_t* offsets, 
		const std::size_t verticies, const std::size_t triangles, const std::size_t count);

	void BindVAO() const;
	void UnBindVAO() const;
};