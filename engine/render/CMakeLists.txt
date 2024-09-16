#--------------------------------------------------------------------------
# render
#--------------------------------------------------------------------------

SET(files_render_display
	window.h
	window.cc
	grid.h
	grid.cc
	mesh.h
	mesh.cc
	texture.h
	texture.cc
	camera.h
	camera.cc
	shader.h
	shader.cc
	)
SOURCE_GROUP("display" FILES ${files_render_display})


SET(files_render
	${files_render_display}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw math-lib)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts glew glfw imgui math-lib ${OPENGL_LIBS})