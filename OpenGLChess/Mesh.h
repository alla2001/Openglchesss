#pragma once
#include <stdint.h>
#include "glew.h"
#include <bitset>
#include <memory>
#include <vector>
#include "objload.h"
class Mesh {

	GLuint m_VAO = 0;
	GLuint m_VBO = 0;
	GLuint m_EBO = 0;
	
	
	std::vector<float> m_vertexdata;
	std::vector<unsigned short> m_elementdata;
	std::vector<float> m_texCoord;
	std::vector<float> m_normal;

	
private:
	int GenVertexArray();
	int BindVertexData();
	int BindElementData();
	int SetVertexAttributePointer();
public:
	int Draw () const;
	Mesh(std::vector<float> texCoord, std::vector<float> vertexdata, std::vector<float> normal, std::vector<unsigned short> elementdata);
	Mesh(obj::Model model);
	~Mesh();
};

