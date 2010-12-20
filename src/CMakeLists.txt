SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Add compiler directives
add_definitions(-Wall)
add_definitions(-Werror=return-type)

ADD_SUBDIRECTORY(second_step)
ADD_SUBDIRECTORY(unit_test)
ADD_SUBDIRECTORY(bwt)

# Create a variable containing all .cpp files
FILE(GLOB files *.cpp)
FILE(GLOB libs *.so)

# Create an executable file from sources
ADD_EXECUTABLE(koda_bwt ${files})
TARGET_LINK_LIBRARIES(koda_bwt ${LIBRARY_OUTPUT_PATH}/libMTF.so ${LIBRARY_OUTPUT_PATH}/libDC.so ${LIBRARY_OUTPUT_PATH}/libIF.so ${LIBRARY_OUTPUT_PATH}/libRLE_2.so ${LIBRARY_OUTPUT_PATH}/libIFC.so ${LIBRARY_OUTPUT_PATH}/libRLE.so ${LIBRARY_OUTPUT_PATH}/libHuffmanCoder.so ${LIBRARY_OUTPUT_PATH}/libBWT.so)
