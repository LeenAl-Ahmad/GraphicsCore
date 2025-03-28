// GraphicsCore.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "GameController.h"

static void AddVector()
{
	glm::vec2 a = { 3,5 };
	glm::vec2 b = { 2,-1 };
	glm::vec2 c = a + b;
	std::cout << glm::to_string(c) << std::endl;

	glm::vec3 d = { 3,5,1 };
	glm::vec3 e = { 2,-1,-1 };
	glm::vec3 f = { d + e };
	std::cout << glm::to_string(f) << std::endl;
}
static void SubtractVectors()
{
	glm::vec2 a = { 3,5 };
	glm::vec2 b = { 2,-1 };
	glm::vec2 c = b - a;
	glm::vec2 d = a - b;
	std::cout << glm::to_string(c) << "Magnitude: " << glm::length(c) << std::endl;
	std::cout << glm::to_string(d) << " Magnitude: " << glm::length(d) << std::endl;
}
static glm::vec3 pos = { 2,2,2 };
static void MultiplyVector()
{
	glm::vec3 dest = { 1,0,0 };
	glm::vec3 dir = dest - pos;
	dir = glm::normalize(dir);
	pos += (dir * 0.1f);
	std::cout << glm::to_string(pos) << std::endl;
}
static void Distance()
{
	glm::vec3 p1 = { 1,1,0 };
	glm::vec3 p2 = { 2,1,2 };
	float distance = glm::distance(p1, p2);
	std::cout << "Distance: " << distance << std::endl;
}
static void DotProduct()
{
	glm::vec3 source = { 0,0,0 };
	glm::vec3 a = { 0,2,0 };
	glm::vec3 b = { 1,1,0 };
	glm::vec3 aVec = a - source;
	glm::vec3 bVec = b - source;
	aVec = glm::normalize(aVec);
	bVec = glm::normalize(bVec);
	float dot = glm::dot(aVec, bVec);
	std::cout << "Dot: " << dot << std::endl;
}
static void CrossProduct()
{
	glm::vec3 source = { 0,0,0 };
	glm::vec3 a = { 0,2,0 };
	glm::vec3 b = { 1,1,0 };
	glm::vec3 aVec = a - source;
	glm::vec3 bVec = b - source;
	glm::vec3 cross = glm::cross(aVec, bVec);
	cross = glm::normalize(cross);
	std::cout << "Cross: " << glm::to_string(cross) << std::endl;
}
int main()
{
	CrossProduct();
	return 0;
}