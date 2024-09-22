#include "config.h"
#include "mesh.h"

#include <cassert>
#include <iostream>
#include <unordered_map>

#include "util/meshDataParser.h"

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
		for (auto& el : groups) {
			el.tex->Unload();
		}
	}

	void Mesh::Draw() const {
		Bind();
		for (const auto& el : groups) {
			el.tex->Bind();
			glDrawElements(GL_TRIANGLES, el.indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * el.offset));
			el.tex->UnBind();
		}
		UnBind();
	}

	void Mesh::Draw(std::size_t i) const {
		Bind();
		groups[i].tex->Bind();
		glDrawElements(GL_TRIANGLES, groups[i].indices, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * groups[i].offset));
		groups[i].tex->UnBind();
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
			-0.5f * width, 0.01f, -0.5f * height,	1.0f, 0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
			-0.5f * width, 0.01f,  0.5f * height,	0.0f, 1.0f, 0.0f, 1.0f,		0.0f, 0.0f,
			 0.5f * width, 0.01f,  0.5f * height,	0.0f, 0.0f, 1.0f, 1.0f,		0.0f, 1.0f,
			 0.5f * width, 0.01f, -0.5f * height,	0.0f, 0.0f, 0.0f, 1.0f,		1.0f, 1.0f
		};

		GLuint ib[] = {
			0, 2, 3,
			0, 1, 2,
		};
		std::size_t sizes[] = { 3, 4, 2 };
		std::size_t offsets[] = { 0, 3, 7 };
		mesh.Init(vb, ib, sizes, offsets, 4, 2, 3);
		auto tex = std::make_shared<Texture>();
		tex->LoadFromFile("../projects/CameraExample/res/img.png");
		mesh.PushPrimitive({ 6, 0, tex });
		return mesh;
	}

	Mesh Mesh::CreateCubeMesh(const float width, const float height, const float depth)
	{
		Mesh mesh{};
		GLfloat vb[] =
		{
			-0.5f * width,	-0.5f * height,	-0.5f * depth,	1, 0, 0, 1,		1.0f, 0.0f,
			-0.5f * width,	 0.5f * height,	-0.5f * depth,	0, 1, 0, 1,		0.0f, 0.0f,
			0.5f * width,	 0.5f * height,	-0.5f * depth,	0, 0, 0, 1,		0.0f, 1.0f,
			0.5f * width,	-0.5f * height,	-0.5f * depth,	0, 0, 1, 1,		1.0f, 1.0f,
			-0.5f * width,	-0.5f * height,	 0.5f * depth,	1, 0, 0, 1,		1.0f, 1.0f,
			-0.5f * width,	 0.5f * height,	 0.5f * depth,	0, 1, 0, 1,		0.0f, 1.0f,
			0.5f * width,	 0.5f * height,	 0.5f * depth,	0, 0, 0, 1,		0.0f, 0.0f,
			0.5f * width,	-0.5f * height,	 0.5f * depth,	0, 0, 1, 1,		1.0f, 0.0f,
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
		std::size_t sizes[] = { 3, 4, 2 };
		std::size_t offsets[] = { 0, 3, 7 };
		mesh.Init(vb, ib, sizes, offsets, 8, 12, 3);
		auto tex = std::make_shared<Texture>();
		tex->LoadFromFile("../projects/CameraExample/res/img.png");
		mesh.PushPrimitive({ 36, 0, tex });
		//mesh.PushPrimitive({ 6, 0,  tex });
		//mesh.PushPrimitive({ 6, 6,  tex});
		//mesh.PushPrimitive({ 6, 12, tex});
		//mesh.PushPrimitive({ 6, 18, tex});
		//mesh.PushPrimitive({ 6, 24, tex});
		//mesh.PushPrimitive({ 6, 30, tex});
		return mesh;
	}

	MeshBuilder::MeshBuilder(const std::string& path) {
		ReadMeshData(path);
	}

	void MeshBuilder::ReadMeshData(const std::string& path) {
		Utils::MeshDataParser parser{};
		Utils::OBJMeshData data = parser.ParseOBJ(path);
		std::unordered_map<std::uint64_t, std::size_t> map{};
	
		for (std::size_t i = 0; i <= data.pos_idx.size() - 3; i += 3) {
			vertexes.push_back(VertexData{
				data.positions[data.pos_idx[i]],
				data.normals[data.norm_idx[i]],
				data.uvs[data.uv_idx[i]]
				});

			vertexes.push_back(VertexData{
				data.positions[data.pos_idx[i + 1]],
				data.normals[data.norm_idx[i + 1]],
				data.uvs[data.uv_idx[i + 1]]
				});

			vertexes.push_back(VertexData{
				data.positions[data.pos_idx[i + 2]],
				data.normals[data.norm_idx[i + 2]],
				data.uvs[data.uv_idx[i + 2]]
				});

			indices.push_back(i);indices.push_back(i + 1);indices.push_back(i + 2);
		}
	}

	Mesh MeshBuilder::CreateMesh(const std::string& texPath) const {
		Mesh mesh{};
		std::size_t sizes[] = {3, 3, 2};
		std::size_t offsets[] = {0, 3, 6};
		mesh.Init((GLfloat*)vertexes.data(), (GLuint*)indices.data(), sizes, offsets, vertexes.size(), indices.size() / 3, 3);
		auto tex = std::make_shared<Texture>();
		tex->LoadFromFile(texPath.c_str());
		mesh.PushPrimitive({ indices.size(), 0, tex });
		return mesh;
	}

	Mesh MeshBuilder::CreateMesh(const std::shared_ptr<Texture> tex) const {
		Mesh mesh{};
		std::size_t sizes[] = { 3, 3, 2 };
		std::size_t offsets[] = { 0, 3, 6 };
		mesh.Init((GLfloat*)vertexes.data(), (GLuint*)indices.data(), sizes, offsets, vertexes.size(), indices.size() / 3, 3);
		mesh.PushPrimitive({ indices.size(), 0, tex });
		return mesh;
	}

} // Resource