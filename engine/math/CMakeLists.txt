#--------------------------------------------------------------------------
# math-lib
#--------------------------------------------------------------------------

SET(files-math-lib
	vec4.h
	vec3.h
	vec2.h
	mat4.h)
SOURCE_GROUP("math-lib" FILES ${files-math-lib})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(math-lib STATIC ${files-math-lib} ${files_pch})
TARGET_PCH(math-lib ../)
ADD_DEPENDENCIES(math-lib glew)
TARGET_LINK_LIBRARIES(math-lib PUBLIC engine exts glew)
