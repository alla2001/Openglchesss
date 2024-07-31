#pragma once
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include <vector>
#include <stdint.h>
#include "Shader.h"
#include <memory>

class GameEntity {
    public :
	glm::vec3 m_position= glm::vec3(0,0,0);
	glm::vec3 m_scale = glm::vec3(1,1,1);
	glm::vec3 m_rotation = glm::vec3(1,1,1);
	glm::mat4x4 m_model = glm::mat4x4();
	unsigned int m_albedoMap;
	unsigned int m_normalMap;
	unsigned int m_metallicMap;
	unsigned int m_roughnessMap;
	unsigned int m_aoMap;

	const Mesh* m_mesh;
	const Shader* m_shader;
	public:
		
		int Init();
		int Draw(glm::mat4 proj, glm::mat4 view, std::vector<Light> lights, glm::vec3 camPos, float deltaTime, unsigned int cubeMap );
		void SetPosition(glm::vec3 position);
		void Scale(glm::vec3 scale);
		void Scale(float x, float y, float z);
		void Rotate(glm::vec3 r);
		GameEntity(Mesh* mesh, Shader* shader);
		int SetMaterialProperties(unsigned int  albedoMap, unsigned int normalMap, unsigned int metallicMap, unsigned int roughnessMap);
		void CalculateModelMat();
		void GraphicsDebugFresnel(bool on);
		void GraphicsDebugGeometry(bool on);
		void GraphicsDebugDistribution(bool on);
		int  DrawShadow(Shader* shadowShader, Light light);
};