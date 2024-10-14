#pragma once

#include <GL/glew.h>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace Resource {

	class Texture {
		GLuint handle;

	public:
		Texture() = default;
		Texture(const std::filesystem::path& path, int flip = 0);
		Texture(const Texture& other);
		~Texture() = default;

		Texture& operator=(const Texture& other);

		void LoadFromFile(const std::filesystem::path& path, int flip = 0);
		void Unload();

		void Bind(GLint loc = 0) const;
		void UnBind() const;
	};

	class TextureManager {
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

	public:
		TextureManager() = default;
		TextureManager(const TextureManager&) = delete;
		TextureManager(TextureManager&&) = delete;
		~TextureManager() = default;

		TextureManager& operator=(const TextureManager&) = delete;
		TextureManager& operator=(TextureManager&&) = delete;

		void Push(const std::string& name, const std::filesystem::path& path, int flip = 0);
		std::weak_ptr<Texture> Get(const std::string& name) const;
	};

} // Resource