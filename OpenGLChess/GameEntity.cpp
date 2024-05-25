#include "GameEntity.h"
#include "GlobalHeader.h"

int GameEntity::Init()
{
    m_model= glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_position);
    m_model = glm::scale(m_model, m_scale);
    return 0;
}

int GameEntity::Draw(glm::mat4 proj, glm::mat4 view, std::vector<Light> lights, glm::vec3 camPos)
{
    int x = 0;
   
    CalculateModelMat();
    m_shader->Use();
    m_shader->setMat4("projection", proj);
    m_shader->setMat4("view", view);
    m_shader->setMat4("model", m_model);
    m_shader->setLights(lights);
    m_shader->setVec3("camPos", camPos);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_albedoMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_normalMap);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_metallicMap);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_roughnessMap);
    ASSERT("Shader Used")

    m_mesh->Draw();
    return 0;
}

void GameEntity::SetPosition(glm::vec3 position)
{
    m_position = position;
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_position);
    m_model = glm::scale(m_model, m_scale);
}

void GameEntity::Scale(glm::vec3 scale)
{
    m_scale= scale;
   
}
void GameEntity::Scale(float x, float y, float z)
{
    m_scale = glm::vec3(x,y,z);
  
}
void GameEntity::Rotate(glm::vec3 r) {
   
    m_rotation = r;
  
}

GameEntity::GameEntity(Mesh* mesh, Shader* shader)
{
    m_mesh = mesh;
    m_shader = shader;
    Init();
}

void GameEntity::CalculateModelMat()
{
    m_model = glm::mat4(1.0f);
    m_model = glm::translate(m_model, m_position);
    m_model = glm::scale(m_model, m_scale);
    m_model = glm::rotate(m_model, m_rotation.x,glm::vec3(1,0,0));
    m_model = glm::rotate(m_model, m_rotation.y, glm::vec3(0, 1, 0));
    m_model = glm::rotate(m_model, m_rotation.z, glm::vec3(0, 0, 1));
}

void GameEntity::GraphicsDebugFresnel(bool on)
{
    m_shader->setBool("debug.fresnel", on);
}
void GameEntity::GraphicsDebugGeometry(bool on)
{
    m_shader->setBool("debug.geometry", on);
}
void GameEntity::GraphicsDebugDistribution(bool on)
{
    m_shader->setBool("debug.distribution", on);
}
int GameEntity::SetMaterialProperties(unsigned int  albedoMap, unsigned int normalMap, unsigned int metallicMap, unsigned int roughnessMap)
{
    m_albedoMap= albedoMap;
    m_normalMap= normalMap;
    m_metallicMap= metallicMap;
    m_roughnessMap = roughnessMap;
   
    return 0;
}

