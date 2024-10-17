#pragma once

#include <vector>

#include "camera.h"
#include "shader.h"
#include "fx/gltf.h"
#include "GL/glew.h"

namespace Resource {

	struct Model {
		struct Mesh {
			struct Primitive {
				GLuint vao;
				GLuint indices;
				GLuint offset = 0;
				GLenum indexType;
				GLenum mode;

				void Draw(const std::shared_ptr<Shader>& shader, const Render::Camera& cam) const;
			};

			std::vector<Primitive> groups;
		};

		struct Buffer {
			GLenum target;
			GLuint handle;
		};

		std::vector<Mesh> meshes;
		std::vector<Buffer> buffers;

		Model() = default;
		Model(const std::filesystem::path& filepath);

		void Draw(const std::weak_ptr<Shader>& shader, const Render::Camera& cam) const;

	private:
		GLuint SlotFromGLTF(const std::string& attribute) const;
	};

} // Resource