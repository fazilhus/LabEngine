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
			std::size_t verticies, std::size_t triangles, std::size_t count);
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
	protected:
		std::vector<VertexData> vertexes;
		std::vector<GLuint> indices;

	public:
		MeshBuilder() = default;
		MeshBuilder(const MeshBuilder&) = delete;
		MeshBuilder(MeshBuilder&&) = delete;
		virtual ~MeshBuilder() = 0;

		MeshBuilder& operator=(const MeshBuilder&) = delete;
		MeshBuilder& operator=(MeshBuilder&&) = delete;

		virtual void ReadMeshData(const std::string& path) = 0;
		virtual Mesh CreateMesh(const std::weak_ptr<Resource::Material>& mat) const = 0;
	};

	class OBJMeshBuilder final : public MeshBuilder {
	public:
		explicit OBJMeshBuilder(const std::string& path);
		~OBJMeshBuilder() override = default;

		void ReadMeshData(const std::string& path) override;
		Mesh CreateMesh(const std::weak_ptr<Resource::Material>& mat) const override;
	};

	class GLTFMeshBuilder final : public MeshBuilder {
		explicit GLTFMeshBuilder(const std::string& path);
		~GLTFMeshBuilder() override = default;

		void ReadMeshData(const std::string& path) override;
		Mesh CreateMesh(const std::weak_ptr<Resource::Material>& mat) const override;
	};

} // Resource