#include "config.h"
#include "model.h"

#include <iostream>

#include "fx/gltf.h"

namespace Resource {

	Model::Model(const std::filesystem::path& filepath) {
		if (!std::filesystem::exists(filepath)) {
			std::cerr << "[ERROR] trying to access non-existing file: " << filepath << '\n';
			return;
		}

		auto doc = fx::gltf::LoadFromText(filepath);
		buffers.resize(doc.bufferViews.size());
		glGenBuffers(buffers.size(), buffers.data());

		for (const auto& [i, buf] : std::views::enumerate(doc.bufferViews)) {
			auto target = static_cast<GLenum>(buf.target);
			glBindBuffer(target, buffers[i]);
			const auto dataPtr = reinterpret_cast<GLbyte*>(doc.buffers[buf.buffer].data.data()) + buf.byteOffset;
			glBufferData(target, buf.byteLength, dataPtr, GL_STATIC_DRAW);
		}
	}

	void Model::Draw(const std::weak_ptr<Shader>& shader) const {

	}
} // Resource