#pragma once

#include <GL/glew.h>

namespace Resource {

	class Texture {
	private:
		GLuint handle;

	public:
		Texture() = default;
		Texture(const Texture& other);
		~Texture() = default;

		Texture& operator=(const Texture& other);

		void LoadFromFile(const char* path);
		void Unload();

		void Bind() const;
		void UnBind() const;
	};

} // Resource