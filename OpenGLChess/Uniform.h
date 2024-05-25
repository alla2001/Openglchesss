#pragma once
#include <string>
template <typename T>
struct Uniform {

	T value;
	string id;
	/*int BindUniform(void* shaderptr) 
	{
		if (typeid(T) == typeid(float)) 
		{

		}
	}*/
};