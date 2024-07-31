


class EnvoirmentMap {
	unsigned int m_cubeMapTexture;
	Shader* m_envoirmentShader;
	unsigned int m_VBO;
	unsigned int m_VAO;
	float verts[18]= {
	0.0f, 0.0f,0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 0.0f,0.0f,
	0.0f, 1.0f,0.0f,
	1.0f, 1.0f, 0.0f,
	};
public:
	void Init(unsigned int envTex,Shader* shader)
	{

		m_cubeMapTexture = envTex;
		m_envoirmentShader = shader;
		glGenBuffers(1,&m_VBO);
		glGenVertexArrays(1, &m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER,sizeof(verts), verts, GL_STATIC_DRAW);
		glBindVertexArray(m_VAO);
	
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void DrawMap(glm::mat4 proj, glm::mat4 view, glm::vec3 camPos)
	{
		m_envoirmentShader->Use();

		m_envoirmentShader->setMat4("projection", proj);
		m_envoirmentShader->setMat4("view", view);

		m_envoirmentShader->setVec3("camPos", camPos);
		glBindVertexArray(m_VAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}


};