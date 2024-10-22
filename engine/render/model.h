#pragma once

#include <vector>

#include "camera.h"
#include "material.h"
#include "shader.h"
#include "texture.h"

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
				std::weak_ptr<Material> material;

				void Draw(const Render::Camera& cam, const Math::mat4& transform) const;
			};

			std::vector<Primitive> groups;
		};

		struct Buffer {
			GLenum target;
			GLuint handle;
		};

		std::vector<Mesh> meshes;
		std::shared_ptr<Texture> dummyTexture;
		std::vector<std::shared_ptr<Texture>> textures;
		std::vector<std::shared_ptr<Material>> materials;
		std::vector<Buffer> buffers;
		Math::mat4 transform;

		Model() = default;
		Model(const std::filesystem::path& filepath, const ShaderManager& sm);

		void Draw(const Render::Camera& cam) const;

	private:
		GLuint SlotFromGLTF(const std::string& attribute) const;
	};

} // Resource