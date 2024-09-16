#--------------------------------------------------------------------------
# GraphicsNodeExample project
#--------------------------------------------------------------------------

PROJECT(GraphicsNodeExample)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("GraphicsNodeExample" FILES ${files_example})

ADD_EXECUTABLE(GraphicsNodeExample ${files_example})
TARGET_LINK_LIBRARIES(GraphicsNodeExample core render)
ADD_DEPENDENCIES(GraphicsNodeExample core render math-lib)