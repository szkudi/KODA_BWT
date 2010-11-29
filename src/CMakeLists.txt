# Create a variable containing all .cpp files
FILE(GLOB files *.cpp)

# Create an executable file from sources
ADD_EXECUTABLE(koda_bwt ${files})

install(
  TARGETS koda_bwt
  RUNTIME DESTINATION bin
)
