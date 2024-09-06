#--------------------------------------------------------------------------
# math-lib test
#--------------------------------------------------------------------------

PROJECT(math-lib-test)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("math-lib-test" FILES ${files_example})

ADD_EXECUTABLE(math-lib-test ${files_example})
TARGET_LINK_LIBRARIES(math-lib-test core math-lib)
ADD_DEPENDENCIES(math-lib-test core math-lib)