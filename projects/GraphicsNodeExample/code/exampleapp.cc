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
"uniform mat4 t;\n"
"uniform mat4 view;\n"
"uniform mat4 persp;\n"
"void main()\n"
"{\n"
"	gl_Position = persp * view * t * vec4(iPos, 1);\n"
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
namespace Example {

	//------------------------------------------------------------------------------
	/**
	*/
	ImGuiExampleApp::ImGuiExampleApp() {
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ImGuiExampleApp::~ImGuiExampleApp() {
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void ImGuiExampleApp::Close() {
		if (this->window->IsOpen())
			this->window->Close();

		Core::App::Close();
		this->mesh.DeInit();
		this->shader.reset();
		delete this->grid;
		delete this->camera;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool ImGuiExampleApp::Open() {
		App::Open();
		this->window = new Display::Window;
		window->SetKeyPressFunction([this](int32, int32, int32, int32) {
				//this->window->Close();
			});

		if (this->window->Open()) {
			// set clear color to gray
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			this->shader = std::make_shared<Resource::Shader>(
				"../projects/GraphicsNodeExample/res/shaders/vertex.glsl",
				"../projects/GraphicsNodeExample/res/shaders/fragment.glsl"
			);

			this->camera = new Render::Camera(0.5f, 4.0f / 3.0f, 0.01f, 100.0f);
			this->camera->SetCameraPosition({ 0, 5, 5 });
			this->camera->SetLookatPosition({ 0, 0, 0 });
			this->camera->SetUpDirection({ 0, 1, 0 });

			this->grid = new Render::Grid();
			Resource::MeshBuilder mb{"../projects/GraphicsNodeExample/res/meshes/cube.obj"};
			this->mesh = mb.CreateMesh("../projects/GraphicsNodeExample/res/textures/minecraft-dirt.png");

			return true;
		}
		return false;
	}





	//------------------------------------------------------------------------------
	/**
	*/
	void ImGuiExampleApp::Run() {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		Math::mat4 gridT{};
		Math::mat4 meshT = Math::scale(0.5f);

		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			this->window->Update();
		
			float angle = (float)glfwGetTime();

			this->camera->SetCameraPosition({cosf(angle) * 10, 5, sinf(angle) * 10});

			auto v = this->camera->GetView();
			auto p = this->camera->GetPerspective();

			this->shader->Use();
			this->shader->UploadUniformMat4fv("t", meshT);
			this->shader->UploadUniformMat4fv("view", v);
			this->shader->UploadUniformMat4fv("persp", p);

			this->mesh.Draw(this->shader->GetHandle());
			this->grid->Draw(&gridT[0][0], &v[0][0], &p[0][0]);

			// transfer new frame to window
			this->window->SwapBuffers();

	#ifdef CI_TEST
			// if we're running CI, we want to return and exit the application after one frame
			// break the loop and hopefully exit gracefully
			break;
	#endif
		}
	}

} // namespace Example