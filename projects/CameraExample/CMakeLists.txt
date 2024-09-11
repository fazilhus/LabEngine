#--------------------------------------------------------------------------
# MeshResourceExample project
#--------------------------------------------------------------------------

PROJECT(MeshResourceExample)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("MeshResourceExample" FILES ${files_example})

ADD_EXECUTABLE(MeshResourceExample ${files_example})
TARGET_LINK_LIBRARIES(MeshResourceExample core render)
ADD_DEPENDENCIES(MeshResourceExample core render)