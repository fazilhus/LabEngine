#pragma once

#include "GL/glew.h"
#include <string>
#include <unordered_map>
#include <memory>

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"

namespace Resource {

	class Shader {
		GLuint handle;
		GLuint vHandle;
		GLuint fHandle;

		std::string vsSrcPath;
		std::string fsSrcPath;
		std::string vsSrc;
		std::string fsSrc;

		std::unordered_map<std::string, GLuint> uniformLoc;

	public:
		Shader(const std::string& vsPath, const std::string& fsPath);
		~Shader();

		void Cleanup();

		void Use() const;

		GLuint GetHandle() const { return handle; }
		std::string& GetVSSrc() { return vsSrc; }
		std::string& GetFSSrc() { return fsSrc; }

		void UploadUniform1i(const std::string& name, GLint v);
		void UploadUniform1f(const std::string& name, float32 v);
		void UploadUniform3fv(const std::string& name, const Math::vec3& v);
		void UploadUniform4fv(const std::string& name, const Math::vec4& v);
		void UploadUniformMat4fv(const std::string& name, const Math::mat4& m);

		void Recompile();

	private:
		void ReadSource(const std::string& path, std::string& dst);
		void CompileAndLink();
		

		GLuint GetOrUpdateUniformLoc(const std::string& name);
	};

	class ShaderManager {
		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

	public:
		ShaderManager() = default;
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager(ShaderManager&&) = delete;
		~ShaderManager() = default;

		ShaderManager& operator=(const ShaderManager&) = delete;
		ShaderManager& operator=(ShaderManager&&) = delete;

		void NewShader(const std::string& name, const std::string& vsPath, const std::string& fsPath);
		std::weak_ptr<Shader> GetShader(const std::string& name) const;

		void RecompileAll();
		void Recompile(const std::string& name);
	};

} // Resource