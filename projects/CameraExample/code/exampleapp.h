#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2022 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "render/mesh.h"
#include "render/grid.h"
#include "math/mat4.h"

namespace Example
{
class ImGuiExampleApp : public Core::App
{
public:
	/// constructor
	ImGuiExampleApp();
	/// destructor
	~ImGuiExampleApp();

	/// open app
	bool Open();
	/// clone app
	void Close();
	/// run app
	void Run();
private:

	/// compile shaders
	void CompileShaders();

	/// show some ui things
	void RenderUI();

	GLuint program;
	GLuint vertexShader;
	GLuint pixelShader;
	Resource::Mesh mesh;
	Render::Grid* grid;
	Math::mat4 camera;
	Display::Window* window;
	GLchar *vsBuffer;
	GLchar *fsBuffer;
	std::string compilerLog;
};
} // namespace Example