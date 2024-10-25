#include "config.h"
#include "model.h"

#include <iostream>

#include "fx/gltf.h"

namespace Resource {

	void Model::Mesh::Primitive::Draw(const Render::Camera& cam, const Math::mat4& transform) const {
		const auto& mat = material.lock();
		const auto& s = mat->GetShader().lock();

		s->Use();

		glBindVertexArray(vao);

		auto normMat = dynamic_cast<NormalMapMaterial*>(mat.get());
		if (normMat) {
			normMat->Use();
		}
		else {
			mat->Use();
		}

		const auto& mvp = cam.GetPerspective() * cam.GetView();
		s->UploadUniformMat4fv("perspective", cam.GetPerspective());
		s->UploadUniformMat4fv("view", cam.GetView());
		s->UploadUniformMat4fv("transform", transform);
		glDrawElements(mode, indices, indexType, (GLvoid*)offset);

		
		glBindVertexArray(0);

		s->UnUse();
	}

	Model::Model(const std::filesystem::path& filepath, const ShaderManager& sm) {
		if (!std::filesystem::exists(filepath)) {
			std::cerr << "[ERROR] trying to access non-existing file: " << filepath << '\n';
			return;
		}

		auto doc = fx::gltf::LoadFromText(filepath);
		buffers.resize(doc.bufferViews.size());
		for (std::size_t i = 0; i < buffers.size(); ++i)
			glGenBuffers(1, &buffers[i].handle);

		for (const auto& [i, buf] : std::views::enumerate(doc.bufferViews)) {
			GLenum target;
			if (buf.target == fx::gltf::BufferView::TargetType::None) {
				for (const auto& mesh : doc.meshes) {
					for (const auto& group : mesh.primitives) {
						if (group.indices == i) {
							target = GL_ELEMENT_ARRAY_BUFFER;
							goto finish;
						}
					}
				}
				target = GL_ARRAY_BUFFER;
			finish:;
			}
			else {
				target = static_cast<GLenum>(buf.target);
			}
			
			buffers[i].target = target;
			glBindBuffer(target, buffers[i].handle);
			const auto dataPtr = reinterpret_cast<GLbyte*>(doc.buffers[buf.buffer].data.data()) + buf.byteOffset;
			glBufferData(target, buf.byteLength, dataPtr, GL_STATIC_DRAW);
		}

		for (const auto& [i, tex] : std::views::enumerate(doc.textures)) {
			textures.push_back(std::make_shared<Texture>(filepath.parent_path(), doc, i));
		}
		dummyTexture = std::make_shared<Texture>(Texture::DummyTexture());

		for (const auto& mat : doc.materials) {
			if (mat.normalTexture.empty()) {
				Material m;
				m.SetAmbient(mat.pbrMetallicRoughness.baseColorFactor);
				m.SetRoughness(mat.pbrMetallicRoughness.roughnessFactor);
				m.SetShininess(mat.pbrMetallicRoughness.metallicFactor);
				if (mat.pbrMetallicRoughness.baseColorTexture.empty()) {
					m.SetDiffuseTex(dummyTexture);
				}
				else {
					m.SetDiffuseTex(textures[mat.pbrMetallicRoughness.baseColorTexture.index]);
				}
				if (mat.pbrMetallicRoughness.metallicRoughnessTexture.empty()) {
					m.SetSpecTex(dummyTexture);
				}
				else {
					m.SetSpecTex(textures[mat.pbrMetallicRoughness.metallicRoughnessTexture.index]);
				}
				m.SetShader(sm.Get("gPass"));
				materials.push_back(std::make_shared<Material>(m));
			}
			else {
				NormalMapMaterial m;
				m.SetAmbient(mat.pbrMetallicRoughness.baseColorFactor);
				m.SetRoughness(mat.pbrMetallicRoughness.roughnessFactor);
				m.SetShininess(mat.pbrMetallicRoughness.metallicFactor);
				if (mat.pbrMetallicRoughness.baseColorTexture.empty()) {
					m.SetDiffuseTex(dummyTexture);
				}
				else {
					m.SetDiffuseTex(textures[mat.pbrMetallicRoughness.baseColorTexture.index]);
				}
				if (mat.pbrMetallicRoughness.metallicRoughnessTexture.empty()) {
					m.SetSpecTex(dummyTexture);
				}
				else {
					m.SetSpecTex(textures[mat.pbrMetallicRoughness.metallicRoughnessTexture.index]);
				}
				if (mat.normalTexture.empty()) {
					m.SetNormTex(dummyTexture);
				}
				else {
					m.SetNormTex(textures[mat.normalTexture.index]);
				}
				m.SetShader(sm.Get("gNormPass"));
				materials.push_back(std::make_shared<NormalMapMaterial>(m));
			}
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
				p.material = materials[group.material];
				m.groups.push_back(p);

				glBindBuffer(buffers[bv].target, buffers[bv].handle);

				glBindVertexArray(0);
			}
			meshes.push_back(m);
		}
	}

	void Model::UnLoad() {
		for (auto& mesh : meshes) {
			for (auto& group : mesh.groups) {
				glDeleteVertexArrays(1, &group.vao);
			}
		}

		for (auto& tex : textures) {
			tex->Unload();
			tex.reset();
		}

		for (auto& buf : buffers) {
			glDeleteBuffers(1, &buf.handle);
		}
	}

	void Model::Draw(const Render::Camera& cam, const Math::mat4& t) const {
		for (const auto& mesh : meshes) {
			for (const auto& group : mesh.groups) {
				group.Draw(cam, t);
			}
		}
	}

	GLuint Model::SlotFromGLTF(const std::string& attribute) const {
		if (attribute == "POSITION") return 0;
		if (attribute == "NORMAL") return 1;
		if (attribute == "TEXCOORD_0") return 2;
		if (attribute == "TANGENT") return 3;
		assert(false && "unsupported vertex attribute");
		return -1;
	}

} // Resource