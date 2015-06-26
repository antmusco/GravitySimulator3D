#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include "glm/glm.hpp"
#include <string>

// Takes a path to a file and extracts the contents to a string.
std::string fileToString(const std::string filepath);


void printMat4(const glm::mat4& mat);

#endif
