#--------------------------------------------------------------------------
# GLTFExample project
#--------------------------------------------------------------------------

PROJECT(GLTFExample)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("GLTFExample" FILES ${files_example})

ADD_EXECUTABLE(GLTFExample ${files_example})
TARGET_LINK_LIBRARIES(GLTFExample core render)
ADD_DEPENDENCIES(GLTFExample core render math)

IF (MSVC)
    set_property(TARGET GLTFExample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF(MSVC)