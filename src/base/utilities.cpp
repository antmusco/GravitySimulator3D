#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

#include "utilities.h"

/**
 * @brief Takes a file and loads its contents into a string.
 * @details Opens a text file and converts all of its contents to a string.
 * 
 * @param filePath Path to the file to be opened.
 * @return The contents of the file as a string.
 */
std::string fileToString(const std::string filePath) {

	// Open the file stream.
	std::ifstream fileStream(filePath);

	// If the file could be opened, extract its contents into a string.
	if(fileStream) {

		std::string fileString((std::istreambuf_iterator<char>(fileStream)),
			(std::istreambuf_iterator<char>()));

		// Close the file stream.
		fileStream.close();

		// Return the string.
		return fileString;

	} else {

		return "";

	}

}



void printMat4(const glm::mat4& mat) {

	printf("\n%3.2e  %3.2e  %3.2e  %3.2e\n%3.2e  %3.2e  %3.2e  %3.2e\n%3.2e %3.2e  %3.2e  %3.2e\n%3.2e  %3.2e  %3.2e  %3.2e\n\n", 
		mat[0][0], mat[0][1], mat[0][2], mat[0][3],
		mat[1][0], mat[1][1], mat[1][2], mat[1][3],
		mat[2][0], mat[2][1], mat[2][2], mat[2][3],
		mat[3][0], mat[3][1], mat[3][2], mat[3][3]);

}