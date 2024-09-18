#include "config.h"
#include "shader.h"

#include <fstream>
#include <iostream>

namespace Resource {

	Shader::Shader(const std::string& vsPath, const std::string& fsPath)
		: handle(0), vHandle(0), fHandle(0) {
		ReadSource(vsPath, vsSrc);
		ReadSource(fsPath, fsSrc);

		CompileAndLink();
	}

	Shader::~Shader() {
	}

	void Shader::Cleanup() {
		if (handle) {
			glDeleteProgram(handle);
			handle = 0;
		}
		if (vHandle) {
			glDeleteShader(vHandle);
			vHandle = 0;
		}
		if (fHandle) {
			glDeleteShader(fHandle);
			fHandle = 0;
		}
	}

	void Shader::Use() const {
		glUseProgram(handle);
	}

	void Shader::UploadUniform1i(const std::string& name, GLint v) {
		GLuint loc = GetOrUpdateUniformLoc(name);
		glUniform1i(loc, v);
	}

	void Shader::UploadUniform4fv(const std::string& name, const Math::vec4& v) {
		GLuint loc = GetOrUpdateUniformLoc(name);
		glUniform4fv(loc, GL_FALSE, &v.x);
	}

	void Shader::UploadUniformMat4fv(const std::string& name, const Math::mat4& m) {
		GLuint loc = GetOrUpdateUniformLoc(name);
		glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0].x);
	}

	void Shader::ReadSource(const std::string& path, std::string& dst) {
		dst.clear();
		std::ifstream in(path, std::ios::in, std::ios::binary);
		if (!in) {
			std::cerr << "[IO ERROR] could not open shader file: " << path << '\n';
			return;
		}

		in.seekg(0, std::ios::end);
		dst.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&dst[0], dst.size());
		in.close();
	}

	void Shader::CompileAndLink() {
		Cleanup();
		std::string compilerLog{};

		vHandle = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* vsrc = vsSrc.c_str();
		GLint len = (GLint)vsSrc.size();
		glShaderSource(vHandle, 1, &vsrc, &len);
		glCompileShader(vHandle);

		// get error log
		GLint shaderLogSize{};
		glGetShaderiv(vHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(vHandle, shaderLogSize, NULL, buf);
			compilerLog.append(buf);
			printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		fHandle = glCreateShader(GL_FRAGMENT_SHADER);
		const GLchar* fsrc = fsSrc.c_str();
		len = (GLint)fsSrc.size();
		glShaderSource(fHandle, 1, &fsrc, &len);
		glCompileShader(fHandle);

		// get error log
		shaderLogSize = 0;
		glGetShaderiv(fHandle, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(fHandle, shaderLogSize, NULL, buf);
			compilerLog.append(buf);
			printf("[FRAGMENT SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

		handle = glCreateProgram();
		glAttachShader(handle, vHandle);
		glAttachShader(handle, fHandle);
		glLinkProgram(handle);

		shaderLogSize = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetProgramInfoLog(handle, shaderLogSize, NULL, buf);
			compilerLog.append(buf);
			printf("[PROGRAM LINK ERROR]: %s", buf);
			delete[] buf;
		}
	}

	GLuint Shader::GetOrUpdateUniformLoc(const std::string& name) {
		if (uniformLoc.find(name) == uniformLoc.end()) {
			uniformLoc[name] = glGetUniformLocation(handle, name.c_str());
		}
		return uniformLoc[name];
	}

} // Resource