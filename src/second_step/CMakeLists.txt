# Create a variable containing all .cpp files
FILE(GLOB files *.cpp)

set(ALG_LIBS DC MTF IF RLE_2 IFC RLE HuffmanCoder)

foreach(alg_lib ${ALG_LIBS})
	ADD_LIBRARY(${alg_lib} SHARED ${alg_lib}.cpp)
endforeach(alg_lib)	
