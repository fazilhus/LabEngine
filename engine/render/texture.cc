#include "config.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../exts/stb/stb_image.h"

namespace Resource {

	Texture::Texture(const Texture& other)
		: handle(other.handle) {
	}

	Texture& Texture::operator=(const Texture& other) {
		this->handle = other.handle;
		return *this;
	}
	void Texture::LoadFromFile(const char* path) {
		int w, h, comp;
		uchar* image = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);

		if (image == nullptr) {
			return;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		float maxAniso{ 1.0f };
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)image);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}

	void Texture::Unload() {
		if (handle != 0) {
			glDeleteTextures(1, &handle);
		}
	}

	void Texture::Bind() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, handle);
		glUniform1i(0, 0);
	}

	void Texture::UnBind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

} // Resource