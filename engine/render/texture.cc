#include "config.h"
#include "texture.h"

#include <iostream>

#include "stb_image.h"

namespace Resource {

	Texture::Texture(const std::string& path, int flip) {
		LoadFromFile(path, flip);
	}
	
	Texture::Texture(const Texture& other)
		: handle(other.handle) {
	}

	Texture& Texture::operator=(const Texture& other) {
		this->handle = other.handle;
		return *this;
	}
	void Texture::LoadFromFile(const std::string& path, int flip) {
		int w, h, comp;
		stbi_set_flip_vertically_on_load(flip);
		uchar* image = stbi_load(path.c_str(), &w, &h, &comp, STBI_rgb);

		if (image == nullptr) {
			return;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		float maxAniso{ 1.0f };
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
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

	void TextureManager::Push(const std::string& name, const std::string& path, int flip) {
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