//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2022 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include "imgui.h"

#include "render/mesh.h"
#include "math/mat4.h"

#define STRING_BUFFER_SIZE 8192

const GLchar* vs =
"#version 460 core\n"
"layout(location=0) in vec3 iPos;\n"
"layout(location=1) in vec2 iUV;\n"
"layout(location=0) out vec4 oColor;\n"
"layout(location=1) out vec2 oUV;\n"
"uniform mat4 view;\n"
"uniform mat4 persp;\n"
"void main()\n"
"{\n"
"	gl_Position = persp * view * vec4(iPos, 1);\n"
"	oColor = vec4(0.0);\n"
"	oUV = iUV;\n"
"}\n";

const GLchar* ps =
"#version 460 core\n"
"layout(location=0) in vec4 iColor;\n"
"layout(location=1) in vec2 iUV;\n"
"layout(location=0) uniform sampler2D tex;\n" 
"out vec4 oColor;\n"
"void main()\n"
"{\n"
"	oColor = texture(tex, iUV);\n"
"}\n";

using namespace Display;
namespace Example
{

//------------------------------------------------------------------------------
/**
*/
ImGuiExampleApp::ImGuiExampleApp() : vertexShader(0), pixelShader(0), program(0)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
ImGuiExampleApp::~ImGuiExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void
ImGuiExampleApp::Close()
{
	if (this->window->IsOpen())
		this->window->Close();

	Core::App::Close();
	this->mesh.DeInit();
	delete this->grid;
	delete this->camera;
}

//------------------------------------------------------------------------------
/**
*/
bool
ImGuiExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
		{
			//this->window->Close();
		});

	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		// create buffer for shader strings
		this->vsBuffer = new GLchar[STRING_BUFFER_SIZE];
		this->fsBuffer = new GLchar[STRING_BUFFER_SIZE];

		// copy the hardcoded shader into buffer
		strncpy_s(this->vsBuffer, STRING_BUFFER_SIZE, vs, STRING_BUFFER_SIZE);
		strncpy_s(this->fsBuffer, STRING_BUFFER_SIZE, ps, STRING_BUFFER_SIZE);

		// compile the shaders in the buffers
		this->CompileShaders();

		this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
		this->camera->SetCameraPosition({ 0, 5, 5 });
		this->camera->SetLookatPosition({ 0, 0, 0 });
		this->camera->SetUpDirection({ 0, 1, 0 });

		//this->mesh = Resource::Mesh::CreateCubeMesh();
		//this->mesh = Resource::Mesh::CreateQuadMesh();
		this->grid = new Render::Grid();
		Resource::MeshBuilder mb{"../projects/CameraExample/res/sphere.obj"};
		this->mesh = mb.CreateMesh("../projects/CameraExample/res/img.png");

		// set ui rendering function
		this->window->SetUiRender([this]()
		{
			this->RenderUI();
		});

		return true;
	}
	return false;
}





//------------------------------------------------------------------------------
/**
*/
void
ImGuiExampleApp::Run()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	while (this->window->IsOpen())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		float angle = (float)glfwGetTime();
		this->camera->SetCameraPosition({cosf(angle) * 10, 10, sinf(angle) * 10});

		glUseProgram(this->program);

		auto v = this->camera->GetView();
		auto p = this->camera->GetPerspective();

		GLuint viewLoc = glGetUniformLocation(this->program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &v[0][0]);
		GLuint perspLoc = glGetUniformLocation(this->program, "persp");
		glUniformMatrix4fv(perspLoc, 1, GL_FALSE, &p[0][0]);

		this->mesh.Draw(this->program);
		this->grid->Draw(&v[0][0], &p[0][0]);

		// transfer new frame to window
		this->window->SwapBuffers();

#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}

//------------------------------------------------------------------------------
/**
*/
void
ImGuiExampleApp::RenderUI()
{
	if (this->window->IsOpen())
	{
		bool show = true;
		// create a new window
		ImGui::Begin("Shader Sources", &show, ImGuiWindowFlags_NoSavedSettings);

		// create text editors for shader code
		ImGui::InputTextMultiline("Vertex Shader", vsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
			ImGuiInputTextFlags_AllowTabInput);
		
		ImGui::InputTextMultiline("Pixel Shader", fsBuffer, STRING_BUFFER_SIZE, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
			ImGuiInputTextFlags_AllowTabInput);

		// apply button
		if (ImGui::Button("Apply"))
		{
			// if pressed we compile the shaders
			this->CompileShaders();
		}
		if (this->compilerLog.length())
		{
			// if compilation produced any output we display it here
			ImGui::TextWrapped(this->compilerLog.c_str());
		}
		// close window
		ImGui::End();
	}
}

//------------------------------------------------------------------------------
/**
*/
void
ImGuiExampleApp::CompileShaders()
{
	// check if shader already exists
	if (this->program)
	{
		glDeleteProgram(this->program);
		this->program = 0;
	}
	if (this->vertexShader)
	{
		glDeleteShader(this->vertexShader);
		this->vertexShader = 0;
	}
	if (this->pixelShader)
	{
		glDeleteShader(this->pixelShader);
		this->pixelShader = 0;
	}

	this->compilerLog.clear();

	// setup vertex shader
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = static_cast<GLint>(std::strlen(this->vsBuffer));
	glShaderSource(this->vertexShader, 1, &this->vsBuffer, &length);
	glCompileShader(this->vertexShader);

	// get error log
	GLint shaderLogSize;
	glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
		this->compilerLog.append(buf);
		printf("[VERTEX SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// setup pixel shader
	this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	length = static_cast<GLint>(std::strlen(this->fsBuffer));
	glShaderSource(this->pixelShader, 1, &this->fsBuffer, &length);
	glCompileShader(this->pixelShader);

	// get error log
	shaderLogSize;
	glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
		this->compilerLog.append(buf);
		printf("[PIXEL SHADER COMPILE ERROR]: %s", buf);
		delete[] buf;
	}

	// create a program object
	this->program = glCreateProgram();
	glAttachShader(this->program, this->vertexShader);
	glAttachShader(this->program, this->pixelShader);
	glLinkProgram(this->program);
	glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
	if (shaderLogSize > 0)
	{
		GLchar* buf = new GLchar[shaderLogSize];
		glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
		this->compilerLog.append(buf);
		printf("[PROGRAM LINK ERROR]: %s", buf);
		delete[] buf;
	}
}

} // namespace Example