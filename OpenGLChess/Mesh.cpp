#include "Mesh.h"
#include <iostream>
#include "GlobalHeader.h"
#include"objload.h";

int Mesh::GenVertexArray()
{
	glGenVertexArrays(1, &m_VAO);
	return 0;
}

int Mesh::BindVertexData()
{
	ASSERT("Vertex Data Set")
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER,m_VBO);
	std::vector<float> data = std::vector<float>();
	for (int i = 0; i < m_vertexdata.size();i+=3) 
	{
		data.push_back(m_vertexdata.at(i));
		data.push_back(m_vertexdata.at(i + 1));
		data.push_back(m_vertexdata.at(i + 2));
	    data.push_back(m_normal.at(i));
		data.push_back(m_normal.at(i + 1));
		data.push_back(m_normal.at(i + 2));
		
		data.push_back(m_texCoord.at(i-(i/3) ));
		data.push_back(m_texCoord.at(i - (i / 3) +1));
		

	}
	glBufferData(GL_ARRAY_BUFFER, (data.size()) * sizeof(float), data.data(), GL_STATIC_DRAW);
	return 0;
}

int Mesh::BindElementData()
{
	ASSERT("Element Data Set")
	glGenBuffers(1, &m_EBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_elementdata.size() * sizeof(m_elementdata[0]),  m_elementdata.data(), GL_STATIC_DRAW);
	return 0;
}

int Mesh::SetVertexAttributePointer()
{

	ASSERT("Vertex Attribute Set")
	
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	return 0;
}

 int Mesh::Draw ()const
{
	ASSERT("Drawing Mesh")
	glBindVertexArray(m_VAO);
	if (m_elementdata.size() <= 0)
	glDrawArrays(GL_TRIANGLES, 0, m_vertexdata.size());
	else
	glDrawElements(GL_TRIANGLES, m_elementdata.size(), GL_UNSIGNED_SHORT, 0);

	return 0;
}

Mesh::Mesh(std::vector<float> texCoord, std::vector<float> vertexdata, std::vector<float> normal, std::vector<unsigned short>
	elementdata)
{
	ASSERT("Mesh Loaded")
	m_vertexdata = vertexdata;
    m_elementdata = elementdata;
	m_texCoord = texCoord;
	m_normal  = normal;
	
	GenVertexArray();
	BindVertexData();
	if(m_elementdata.size()>0)
		BindElementData();
	
	SetVertexAttributePointer();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
Mesh::Mesh(obj::Model model)
{
	m_vertexdata = model.vertex;
	m_elementdata = model.faces["default"];
	m_texCoord = model.texCoord;
	m_normal = model.normal;
	//std::cout<<model;
	GenVertexArray();
	BindVertexData();
	if (m_elementdata.size() > 0)
		BindElementData();

	SetVertexAttributePointer();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Mesh::~Mesh()
{
	
	ASSERT("Mesh Deleted")
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	
}

//Mesh Mesh::operator=(const obj::Model model) {
//
//	return Mesh();
//}


