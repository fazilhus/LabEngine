#pragma once

#include <GL/glew.h>

#include <string>

namespace Resource {

	class Texture {
		GLuint handle;

	public:
		Texture() = default;
		Texture(const std::string& path, int flip = 0);
		Texture(const Texture& other);
		~Texture() = default;

		Texture& operator=(const Texture& other);

		void LoadFromFile(const std::string& path, int flip = 0);
		void Unload();

		void Bind(GLuint loc = 0) const;
		void UnBind() const;
	};

} // Resource