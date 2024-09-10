#include "config.h"
#include "mesh.h"

Mesh::Mesh()
	: vao(0), vbo(0), ebo(0) {}

void Mesh::Init(GLfloat* vb, GLuint* ib, const std::size_t* sizes, const std::size_t* offsets, 
	const std::size_t verticies, const std::size_t triangles, const std::size_t count) {
	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	std::size_t stride{ 0 };
	for (std::size_t i = 0; i < count; ++i) stride += sizes[i];

	glGenBuffers(1, &this->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * stride * verticies, vb, GL_STATIC_DRAW);

	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 3 * triangles, ib, GL_STATIC_DRAW);

	for (std::size_t i = 0; i < count; ++i) {
		glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, sizeof(GLfloat) * stride, (GLvoid*)(sizeof(GLfloat) * offsets[i]));
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::BindVAO() const {
	glBindVertexArray(this->vao);
}

void Mesh::UnBindVAO() const {
	glBindVertexArray(0);
}
