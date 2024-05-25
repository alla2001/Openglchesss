


class EnvoirmentMap {
	unsigned int cubeMapTexture;
	Shader* envoirmentShader;
	unsigned int m_VBO;
	unsigned int m_VAO;
	float verts[18] = {
		0.0f, 0.0f,0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,0.0f,
		0.0f, 1.0f,0.0f,
		1.0f, 1.0f, 1.0f,
	};
public:
	void Init(unsigned int envTex,Shader* shader)
	{
		cubeMapTexture = envTex;
		
		glGenBuffers(1,&m_VBO);
		glGenVertexArrays(1, &m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), verts, GL_STATIC_DRAW);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		
	}
	void DrawMap() 
	{
		envoirmentShader->Use();
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}


};