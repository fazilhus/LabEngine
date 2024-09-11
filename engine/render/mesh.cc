#include "config.h"
#include "mesh.h"

namespace Resource {

	Mesh::Mesh()
		: vao(0), vbo(0), ebo(0) {}

	Mesh::Mesh(const Mesh& other)
		: vao(other.vao), vbo(other.vbo), ebo(other.ebo), groups(other.groups) {
	}

	Mesh& Mesh::operator=(const Mesh& other) {
		this->vao = other.vao;
		this->vbo = other.vbo;
		this->ebo = other.ebo;
		this->groups = other.groups;
		return *this;
	}

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

	void Mesh::DeInit() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}

	void Mesh::Draw(const GLuint programHandle) const {
		Bind();
		for (auto el : groups) {
			GLuint loc = glGetUniformLocation(programHandle, "face_col");
			glUniform4fv(loc, 1, &el.color[0]);
			glDrawElements(GL_TRIANGLES, el.indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * el.offset));
		}
		UnBind();
	}

	void Mesh::Draw(std::size_t i) const {
		Bind();
		glDrawElements(GL_TRIANGLES, groups[i].indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * groups[i].offset));
		UnBind();
	}

	void Mesh::PushPrimitive(PrimitiveGroup group) {
		groups.push_back(group);
	}

	void Mesh::Bind() const {
		glBindVertexArray(this->vao);
	}

	void Mesh::UnBind() const {
		glBindVertexArray(0);
	}

	Mesh Mesh::CreateQuadMesh(const float width, const float height) {
		Mesh mesh{};
		GLfloat vb[] =
		{
			-0.5f * width,	-0.5f * height,	-1,		
			1,		0,		0,		1,
			-0.5f * width,	0.5f * height,	-1,		
			0,		1,		0,		1,
			0.5f * width,	0.5f * height,	-1,		
			0,		0,		1,		1, 
			0.5f * width,	-0.5f * height,	-1,		
			0,		0,		0,		1,
		};

		GLuint ib[] = {
			0, 1, 2,
			0, 2, 3,
		};
		std::size_t sizes[] = { 3, 4 };
		std::size_t offsets[] = { 0, 3 };
		mesh.Init(vb, ib, sizes, offsets, 4, 2, 2);
		return mesh;
	}

	Mesh Mesh::CreateQubeMesh(const float width, const float height, const float depth)
	{
		Mesh mesh{};
		GLfloat vb[] =
		{
			-0.5f * width,	-0.5f * height,	-0.5f * depth, // 0
			1,		0,		0,		1,
			-0.5f * width,	 0.5f * height,	-0.5f * depth, // 1
			0,		1,		0,		1,
			0.5f * width,	 0.5f * height,	-0.5f * depth, // 2
			0,		0,		0,		1,
			0.5f * width,	-0.5f * height,	-0.5f * depth, // 3
			0,		0,		1,		1,
			-0.5f * width,	-0.5f * height,	 0.5f * depth, // 4
			1,		0,		0,		1,		 
			-0.5f * width,	 0.5f * height,	 0.5f * depth, // 5
			0,		1,		0,		1,		 
			0.5f * width,	 0.5f * height,	 0.5f * depth, // 6
			0,		0,		0,		1,
			0.5f * width,	-0.5f * height,	 0.5f * depth, // 7
			0,		0,		1,		1,		 
		};

		GLuint ib[] = {
			0, 2, 3,
			0, 1, 2,
			0, 5, 1,
			0, 4, 5,
			2, 6, 3,
			3, 6, 7,
			0, 7, 4,
			0, 3, 7,
			1, 6, 2,
			1, 5, 6,
			4, 6, 5,
			4, 7, 6
		};
		std::size_t sizes[] = { 3, 4 };
		std::size_t offsets[] = { 0, 3 };
		mesh.Init(vb, ib, sizes, offsets, 8, 12, 2);
		mesh.PushPrimitive({ 6, 0,  Math::vec4{1, 0, 0, 1} });
		mesh.PushPrimitive({ 6, 6,  Math::vec4{0, 1, 0, 1} });
		mesh.PushPrimitive({ 6, 12, Math::vec4{0, 0, 1, 1} });
		mesh.PushPrimitive({ 6, 18, Math::vec4{1, 0, 1, 1} });
		mesh.PushPrimitive({ 6, 24, Math::vec4{0, 1, 1, 1} });
		mesh.PushPrimitive({ 6, 30, Math::vec4{1, 1, 0, 1} });
		return mesh;
	}

} // Render