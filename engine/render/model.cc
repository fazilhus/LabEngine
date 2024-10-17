#include "config.h"
#include "model.h"

#include <iostream>

#include "fx/gltf.h"

namespace Resource {

	void Model::Mesh::Primitive::Draw(const std::shared_ptr<Shader>& shader, const Render::Camera& cam) const {
		glBindVertexArray(vao);

		auto mat = cam.GetPerspective() * cam.GetView();

		shader->UploadUniformMat4fv("mvp", mat);
		glDrawElements(mode, indices, indexType, (GLvoid*)offset);

		glBindVertexArray(0);
	}

	Model::Model(const std::filesystem::path& filepath) {
		if (!std::filesystem::exists(filepath)) {
			std::cerr << "[ERROR] trying to access non-existing file: " << filepath << '\n';
			return;
		}

		auto doc = fx::gltf::LoadFromText(filepath);
		buffers.resize(doc.bufferViews.size());
		for (std::size_t i = 0; i < buffers.size(); ++i)
			glGenBuffers(1, &buffers[i].handle);

		for (const auto& [i, buf] : std::views::enumerate(doc.bufferViews)) {
			auto target = static_cast<GLenum>(buf.target);
			buffers[i].target = target;
			glBindBuffer(target, buffers[i].handle);
			const auto dataPtr = reinterpret_cast<GLbyte*>(doc.buffers[buf.buffer].data.data()) + buf.byteOffset;
			glBufferData(target, buf.byteLength, dataPtr, GL_STATIC_DRAW);
		}

		for (const auto& mesh : doc.meshes) {
			Mesh m;
			for (const auto& group : mesh.primitives) {
				Mesh::Primitive p;
				glGenVertexArrays(1, &p.vao);
				glBindVertexArray(p.vao);
				for (const auto& [attribute, acc_idx] : group.attributes) {
					const auto accessor = doc.accessors[acc_idx];
					glBindBuffer(buffers[accessor.bufferView].target, buffers[accessor.bufferView].handle);

					auto slot = SlotFromGLTF(attribute);
					glEnableVertexArrayAttrib(p.vao, slot);
					GLuint size = 0;
					switch (accessor.type) {
						case fx::gltf::Accessor::Type::Vec2: {
							size = 2;
							break;
						}
						case fx::gltf::Accessor::Type::Vec3: {
							size = 3;
							break;
						}
						case fx::gltf::Accessor::Type::Vec4: {
							size = 4;
							break;
						}
						default: {
							assert(false && "unsupported accessor type");
						}
					}
					glVertexAttribPointer(
						slot,
						size,
						(GLenum)accessor.componentType,
						accessor.normalized,
						0,
						(GLvoid*)accessor.byteOffset);
				}

				const auto accessor = doc.accessors[group.indices];
				const auto& bv = accessor.bufferView;

				p.indices = accessor.count;
				p.offset = accessor.byteOffset;
				p.mode = (GLenum)group.mode;
				p.indexType = (GLenum)accessor.componentType;
				m.groups.push_back(p);

				glBindBuffer(buffers[bv].target, buffers[bv].handle);

				//glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			meshes.push_back(m);
		}
	}

	void Model::Draw(const std::weak_ptr<Shader>& shader, const Render::Camera& cam) const {
		const auto& s = shader.lock();
		s->Use();

		for (const auto& mesh : meshes) {
			for (const auto& group : mesh.groups) {
				group.Draw(s, cam);
			}
		}

		s->UnUse();
	}

	GLuint Model::SlotFromGLTF(const std::string& attribute) const {
		if (attribute == "POSITION") return 0;
		if (attribute == "NORMAL") return 1;
		if (attribute == "TEXCOORD_0") return 3;
		if (attribute == "TANGENT") return 4;
		assert(false && "unsupported vertex attribute");
		return -1;
	}

} // Resource