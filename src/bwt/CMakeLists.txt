# Create a variable containing all .cpp files
FILE(GLOB files *.cpp)

set(ALG_LIBS BWT)

foreach(alg_lib ${ALG_LIBS})
	ADD_LIBRARY(${alg_lib} SHARED ${alg_lib}.cpp)
endforeach(alg_lib)	
	
