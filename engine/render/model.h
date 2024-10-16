#pragma once

#include <filesystem>
#include <vector>

#include "shader.h"
#include "GL/glew.h"

namespace Resource {

	class GLTFMesh {
	public:
		struct Primitive {
			GLuint vao;
			GLuint indices;
			GLuint offset = 0;
			GLenum indexType;
		};

	private:
		std::vector<Primitive> groups;

	public:
		GLTFMesh() = default;
		GLTFMesh(const std::filesystem::path& filepath);
		GLTFMesh(const GLTFMesh&) = default;
		GLTFMesh(GLTFMesh&&) noexcept = default;
		~GLTFMesh() = default;

		GLTFMesh& operator=(const GLTFMesh&) = default;
		GLTFMesh& operator=(GLTFMesh&&) noexcept = default;

	};

	class Model {
		std::vector<GLTFMesh> meshes;
		std::vector<GLuint> buffers;

	public:
		Model() = default;
		Model(const std::filesystem::path& filepath);
		Model(const Model&) = default;
		Model(Model&&) noexcept = default;
		~Model() = default;

		Model& operator=(const Model&) = default;
		Model& operator=(Model&&) noexcept = default;

		void Draw(const std::weak_ptr<Shader>& shader) const;
	};

} // Resource