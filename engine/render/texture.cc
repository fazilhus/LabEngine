#include "config.h"
#include "texture.h"

#include <iostream>

#include "stb_image.h"
#include "fx/gltf.h"

namespace Resource {

	Texture::Texture(const std::filesystem::path& path, int flip) {
		SetDefaultSampling();
		LoadFromFile(path, flip);
	}

	Texture::Texture(const std::filesystem::path& dir, const fx::gltf::Document& doc, int tex_i, int flip) {
		SetDefaultSampling();
		if (!doc.samplers.empty()) {
			const auto& sampler = doc.samplers[doc.textures[tex_i].sampler];

			wrap = { (GLint)sampler.wrapS, (GLint)sampler.wrapT };

			if (sampler.minFilter == fx::gltf::Sampler::MinFilter::None) {
				filter.min = GL_LINEAR;
			}
			else {
				filter.min = (GLint)sampler.minFilter;
			}

			if (sampler.magFilter == fx::gltf::Sampler::MagFilter::None) {
				filter.mag = GL_LINEAR;
			}
			else {
				filter.mag = (GLint)sampler.magFilter;
			}
		}
		LoadFromGLTF(dir, doc, tex_i, flip);
	}

	Texture::Texture(const Texture& other)
		: handle(other.handle) {
	}

	Texture Texture::DummyTexture() {
		Texture t;
		t.SetDefaultSampling();

		glCreateTextures(GL_TEXTURE_2D, 1, &t.handle);
		glBindTexture(GL_TEXTURE_2D, t.handle);

		GLubyte data[3];
		data[0] = 255;
		data[1] = 255;
		data[2] = 255;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 1, 1, 0,
			GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)data);

		glBindTexture(GL_TEXTURE_2D, 0);
		return t;
	}

	Texture& Texture::operator=(const Texture& other) {
		this->handle = other.handle;
		return *this;
	}

	void Texture::Unload() {
		if (handle != 0) {
			glDeleteTextures(1, &handle);
		}
	}

	void Texture::Bind(GLint loc) const {
		//glUniform1i(GL_TEXTURE0, loc);
		glActiveTexture(GL_TEXTURE0 + loc);
		glBindTexture(GL_TEXTURE_2D, handle);
	}

	void Texture::UnBind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::LoadFromFile(const std::filesystem::path& path, int flip) {
		if (!std::filesystem::exists(path)) {
			std::cerr << "[ERROR] trying to read non-existing texture file: " << path << '\n';
			return;
		}

		int w, h, comp;
		stbi_set_flip_vertically_on_load(flip);
		uchar* image = stbi_load(path.string().c_str(), &w, &h, &comp, STBI_rgb_alpha);

		if (image == nullptr) {
			return;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap.s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap.s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter.min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter.mag);

		float maxAniso{ 1.0f };
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);

		GenerateMipMapifNeeded();

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}

	void Texture::LoadFromGLTF(const std::filesystem::path& dir, const fx::gltf::Document& doc, int tex_i, int flip) {
		const auto& path = dir / std::filesystem::path(doc.images[doc.textures[tex_i].source].uri).make_preferred();
		LoadFromFile(path, flip);
	}

	void Texture::SetDefaultSampling() {
		wrap = { GL_REPEAT, GL_REPEAT };
		filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
	}

	void Texture::GenerateMipMapifNeeded() const {
		switch (filter.min) {
		case GL_NEAREST_MIPMAP_NEAREST:
		case GL_NEAREST_MIPMAP_LINEAR:
		case GL_LINEAR_MIPMAP_NEAREST:
		case GL_LINEAR_MIPMAP_LINEAR: {
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		}
		default: break;
		}
	}

	void TextureManager::Push(const std::string& name, const std::filesystem::path& path, int flip) {
		if (textures.contains(name)) {
			std::cerr << "[WARNING] Overwriting existing shader " << name << '\n';
			textures[name]->Unload();
			textures[name].reset();
		}

		textures[name] = std::make_shared<Texture>(path, flip);
	}

	std::weak_ptr<Texture> TextureManager::Get(const std::string& name) const {
		if (!textures.contains(name)) {
			std::cerr << "[ERROR] Trying to access nonexistent texture " << name << '\n';
			return {};
		}

		return textures.at(name);
	}
} // Resource