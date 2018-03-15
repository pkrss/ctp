#include "fileSave.h"

#include <iostream>
#include <fstream>

void myFileWrite(const char *path, const char *content){
	std::ofstream of(path, std::ofstream::binary);
		of.write(content, strlen(content));
		of.close();
}

// return value need call free()
const char *myFileRead(const char *path){
	std::ifstream infile (path,std::ifstream::binary);

	char *buffer = 0;

	// get size of file
	infile.seekg(0, infile.end);
	long size = infile.tellg();

	if(size >= 0){
		infile.seekg(0);

		// allocate memory for file content
		buffer = malloc(size * sizeof(char));

		// read content of infile
		infile.read(buffer, size);

		// release dynamically-allocated memory
		//   free buffer;
	}
	

	infile.close();
	return buffer;
}