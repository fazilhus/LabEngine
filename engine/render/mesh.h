#pragma once

#include "gl/glew.h"

#include <vector>
#include "math/vec4.h"

struct PrimitiveGroup {
	std::size_t indices;
	std::size_t offset;
	Math::vec4 color;
};

class Mesh {
private:
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	std::vector<PrimitiveGroup> groups;

public:
	Mesh();
	Mesh(const Mesh& other);
	~Mesh() = default;

	Mesh& operator=(const Mesh& other);

	void Init(GLfloat* vb, GLuint* ib, const std::size_t* sizes, const std::size_t* offsets, 
		const std::size_t verticies, const std::size_t triangles, const std::size_t count);
	void DeInit();

	void Draw(const GLuint programHandle) const;
	void Draw(std::size_t i) const;

	void PushPrimitive(PrimitiveGroup group);

	static Mesh CreateQuadMesh(const float width = 1.0f, const float height = 1.0f);
	static Mesh CreateQubeMesh(const float width = 1.0f, const float height = 1.0f, const float depth = 1.0f);

private:
	void Bind() const;
	void UnBind() const;
};