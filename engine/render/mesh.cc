#include "config.h"
#include "mesh.h"

#include <cassert>
#include <iostream>

#include "util/meshDataParser.h"

#include "fx/gltf.h"

namespace Resource {

	Mesh::Mesh()
		: vao(0), vbos(), ebo(0) {}

	Mesh::Mesh(const Mesh& other)
		: vao(other.vao), ebo(other.ebo), groups(other.groups) {
		for (std::size_t i = 0; i < 3; ++i) {
			this->vbos[i] = other.vbos[i];
		}
	}

	Mesh& Mesh::operator=(const Mesh& other) {
		this->vao = other.vao;
		for (std::size_t i = 0; i < 3; ++i) {
			this->vbos[i] = other.vbos[i];
		}
		this->ebo = other.ebo;
		this->groups = other.groups;
		return *this;
	}

	void Mesh::Init(const VertexData& vb, const std::vector<GLuint>& ib, const std::size_t* sizes,
		const std::size_t* offsets, std::size_t count) {
		// TODO maybe split the vertex data buffer into separate buffers (e.g pos, norm and uv buffers)
		glGenVertexArrays(1, &this->vao);
		glBindVertexArray(this->vao);

		std::size_t stride{ 0 };
		for (std::size_t i = 0; i < count; ++i) stride += sizes[i];

		glGenBuffers(3, vbos);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * sizes[0] * vb.pos.size(),
			vb.pos.data(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(
			0,
			sizes[0],
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * sizes[1] * vb.norm.size(),
			vb.norm.data(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(
			1,
			sizes[1],
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(GLfloat) * sizes[2] * vb.uv.size(),
			vb.uv.data(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(
			2,
			sizes[2],
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);


		glGenBuffers(1, &this->ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(GLuint) * ib.size(),
			ib.data(), 
			GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void Mesh::DeInit() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(3, vbos);
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
		if (i >= groups.size()) {
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
		for (std::size_t i = 0; i < 3; ++i) {
			glEnableVertexAttribArray(i);
		}
	}

	void Mesh::UnBind() const {
		glBindVertexArray(0);
	}

	void VertexData::clear() {
		pos.clear();
		norm.clear();
		uv.clear();
	}

	void VertexData::reserve(std::size_t a, std::size_t b, std::size_t c) {
		pos.reserve(a);
		norm.reserve(b);
		uv.reserve(c);
	}

	void VertexData::push_back(Math::vec3 p, Math::vec3 n, Math::vec2 u) {
		pos.push_back(p);
		norm.push_back(n);
		uv.push_back(u);
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
		mesh.Init(vertexes, indices, sizes, offsets, 3);
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