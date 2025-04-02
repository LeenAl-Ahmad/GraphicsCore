// GraphicsCore.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GameController.h"

int main()
{
	glm::mat4 matrix = glm::mat4(1.0f);
	float angle = 0.1f;
	glm::vec3 axis = { 0, 1, 0 };
	matrix = glm::rotate(matrix, angle, axis);
	std::cout << "Rotation Matrix: " << glm::to_string(matrix) << std::endl;
	
	return 0;
}