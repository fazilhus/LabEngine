#--------------------------------------------------------------------------
# CameraExample project
#--------------------------------------------------------------------------

PROJECT(CameraExample)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("CameraExample" FILES ${files_example})

ADD_EXECUTABLE(CameraExample ${files_example})
TARGET_LINK_LIBRARIES(CameraExample core render)
ADD_DEPENDENCIES(CameraExample core render)