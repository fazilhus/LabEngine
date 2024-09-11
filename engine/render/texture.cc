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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
	}

	void Texture::Unload() {
		if (handle != 0) {
			glDeleteTextures(1, &handle);
		}
	}

	void Texture::Bind() const {
		glBindTexture(GL_TEXTURE_2D, handle);
	}

	void Texture::UnBind() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

} // Resource