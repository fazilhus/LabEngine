#pragma once

#include "GL/glew.h"
#include <string>
#include <unordered_map>

#include "config.h"

#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include "render/light.h"

namespace Resource {

	class Shader {
	private:
		GLuint handle;
		GLuint vHandle;
		GLuint fHandle;

		std::string vsSrc;
		std::string fsSrc;

		std::unordered_map<std::string, GLuint> uniformLoc;

		Render::OmniDirLight light;

	public:
		Shader(const std::string& vsPath, const std::string& fsPath);
		~Shader();

		void Cleanup();

		void Use() const;
		void SetLight();
		inline const Render::OmniDirLight& GetLight() const { return light; }
		inline Render::OmniDirLight& GetLight() { return light; }

		inline GLuint GetHandle() const { return handle; }
		inline std::string& GetVSSrc() { return vsSrc; }
		inline std::string& GetFSSrc() { return fsSrc; }

		void UploadUniform1i(const std::string& name, GLint v);
		void UploadUniform1f(const std::string& name, float32 v);
		void UploadUniform3fv(const std::string& name, const Math::vec3& v);
		void UploadUniform4fv(const std::string& name, const Math::vec4& v);
		void UploadUniformMat4fv(const std::string& name, const Math::mat4& m);

	private:
		void ReadSource(const std::string& path, std::string& dst);
		void CompileAndLink();
		

		GLuint GetOrUpdateUniformLoc(const std::string& name);
	};

} // Resource