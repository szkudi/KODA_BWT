SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Add compiler directives
add_definitions(-Wall)
add_definitions(-Werror=return-type)

ADD_SUBDIRECTORY(second_step)
ADD_SUBDIRECTORY(unit_test)