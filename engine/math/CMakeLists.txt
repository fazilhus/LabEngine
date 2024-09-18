#--------------------------------------------------------------------------
# math
#--------------------------------------------------------------------------

SET(files-math
	vec4.h
	vec3.h
	vec2.h
	mat4.h)
SOURCE_GROUP("math" FILES ${files-math})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(math STATIC ${files-math} ${files_pch})
TARGET_PCH(math ../)
ADD_DEPENDENCIES(math glew)
TARGET_LINK_LIBRARIES(math PUBLIC engine exts glew)
