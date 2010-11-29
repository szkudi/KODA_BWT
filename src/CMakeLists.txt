SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

ADD_SUBDIRECTORY(second_step)

# Add compiler directives
add_definitions(-Wall)
add_definitions(-Werror=return-type)