#--------------------------------------------------------------------------
# LightingExample project
#--------------------------------------------------------------------------

PROJECT(LightingExample)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("LightingExample" FILES ${files_example})

ADD_EXECUTABLE(LightingExample ${files_example})
TARGET_LINK_LIBRARIES(LightingExample core render)
ADD_DEPENDENCIES(LightingExample core render math)