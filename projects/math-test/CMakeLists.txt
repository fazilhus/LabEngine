#--------------------------------------------------------------------------
# math test
#--------------------------------------------------------------------------

PROJECT(math-test)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("math-test" FILES ${files_example})

ADD_EXECUTABLE(math-test ${files_example})
TARGET_LINK_LIBRARIES(math-test core math)
ADD_DEPENDENCIES(math-test core math)

IF (MSVC)
    set_property(TARGET math-test PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
ENDIF(MSVC)