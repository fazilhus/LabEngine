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
	model.h
	model.cc
	texture.h
	texture.cc
	camera.h
	camera.cc
	shader.h
	shader.cc
	node.h
	node.cc
	material.h
	material.cc
	light.h
	light.cc
	)
SOURCE_GROUP("display" FILES ${files_render_display})


SET(files_render
	${files_render_display}
	${files_render_pch})

SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(render STATIC ${files_render} ${files_pch})
TARGET_PCH(render ../)	
ADD_DEPENDENCIES(render glew glfw math stb_image)
TARGET_LINK_LIBRARIES(render PUBLIC engine exts stb_image glew glfw imgui math ${OPENGL_LIBS})