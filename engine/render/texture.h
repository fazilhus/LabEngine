#pragma once

#include <GL/glew.h>

#include <string>

namespace Resource {

	class Texture {
	private:
		GLuint handle;

	public:
		Texture() = default;
		Texture(const std::string& path);
		Texture(const Texture& other);
		~Texture() = default;

		Texture& operator=(const Texture& other);

		void LoadFromFile(const std::string& path);
		void Unload();

		void Bind() const;
		void UnBind() const;
	};

} // Resource