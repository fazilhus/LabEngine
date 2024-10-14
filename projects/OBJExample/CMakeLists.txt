#--------------------------------------------------------------------------
# OBJExample project
#--------------------------------------------------------------------------

PROJECT(OBJExample)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("OBJExample" FILES ${files_example})

ADD_EXECUTABLE(OBJExample ${files_example})
TARGET_LINK_LIBRARIES(OBJExample core render)
ADD_DEPENDENCIES(OBJExample core render math)

IF (MSVC)
    set_property(TARGET OBJExample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF(MSVC)