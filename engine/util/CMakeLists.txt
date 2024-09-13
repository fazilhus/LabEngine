#--------------------------------------------------------------------------
# util
#--------------------------------------------------------------------------

SET(files_util
	meshDataParser.h)
SOURCE_GROUP("util" FILES ${files_util})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(util STATIC ${files_util} ${files_pch})
TARGET_PCH(util ../)
TARGET_LINK_LIBRARIES(util PUBLIC engine exts math)
