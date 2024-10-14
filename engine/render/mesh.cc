#include "config.h"
#include "mesh.h"

#include <cassert>
#include <iostream>

#include "util/meshDataParser.h"

#include "fx/gltf.h"

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
		// TODO maybe split the vertex data buffer into separate buffers (e.g pos, norm and uv buffers)
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

	void Mesh::Draw() const {
		Bind();
		for (const auto& el : groups) {
			if (!el.mat.expired()) {
				el.mat.lock()->Use();
			}
			glDrawElements(GL_TRIANGLES, el.indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * el.offset));
		}
		UnBind();
	}

	void Mesh::DrawGroup(std::size_t i) const {
		if (i > groups.size()) {
			std::cerr << "[ERROR] attempting to draw " << i + 1 << "th primitive group, when only " << groups.size() << " exist\n";
			return;
		}

		Bind();
		if (!groups[i].mat.expired()) {
			groups[i].mat.lock()->Use();
		}
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

	OBJMeshBuilder::OBJMeshBuilder(const std::string& path) {
		ReadMeshData(path);
	}

	void OBJMeshBuilder::ReadMeshData(const std::string& path) {
		Utils::MeshDataParser parser{};
		vertexes.clear();
		indices.clear();
		parser.ParseOBJ(path, vertexes, indices);
	}

	Mesh OBJMeshBuilder::CreateMesh() const {
		Mesh mesh{};
		std::size_t sizes[] = { 3, 3, 2 };
		std::size_t offsets[] = { 0, 3, 6 };
		mesh.Init((GLfloat*)vertexes.data(), (GLuint*)indices.data(), sizes, offsets, vertexes.size(), indices.size() / 3, 3);
		mesh.PushPrimitive({ indices.size(), 0, {} });
		return mesh;
	}

	GLTFMeshBuilder::GLTFMeshBuilder(const std::string& path_str) {
		auto path = std::filesystem::path(path_str);
		if (!is_regular_file(path)) {
			std::cerr << "[ERROR] file " << path_str << " does not exist\n";
			return;
		}

		
	}

	void GLTFMeshBuilder::ReadMeshData(const std::string& path) {
	}

	Mesh GLTFMeshBuilder::CreateMesh() const {
		return {};
	}

} // Resource