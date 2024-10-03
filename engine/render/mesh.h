#pragma once

#include "gl/glew.h"

#include <vector>
#include <string>
#include <fstream>

#include "math/vec2.h"
#include "math/vec3.h"
#include "material.h"

namespace Resource {

	struct PrimitiveGroup {
		std::size_t indices;
		std::size_t offset;
		std::weak_ptr<Material> mat;
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

		void Draw() const;
		void DrawGroup(std::size_t i) const;

		void PushPrimitive(PrimitiveGroup group);

	private:
		void Bind() const;
		void UnBind() const;
	};

	struct VertexData {
		Math::vec3 pos;
		Math::vec3 norm;
		Math::vec2 uv;
	};

	class MeshBuilder {
	private:
		std::vector<VertexData> vertexes;
		std::vector<GLuint> indices;

	public:
		MeshBuilder(const std::string& path);
		~MeshBuilder() = default;

		void ReadMeshData(const std::string& path);
		Mesh CreateMesh(const std::shared_ptr<Resource::Material>& mat) const;
	};

} // Resource