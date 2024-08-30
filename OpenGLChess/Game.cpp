#include "GlobalHeader.h"
#define GLEW_STATIC

#include "glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include "Window.h"
#include "GameEntity.h"
#include <memory>
#include "AssetLoader.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <array>
#include <chrono>
#include "UIManager.hpp"
#include "EnvoirmentMap.hpp"
#include "ShadowMap.hpp"
#include "Camera.h"
#include "DungeonGenerator.h"


void ResizeCallBack(GLFWwindow* window, int w, int h);
void ProcessInputs();
Window m_window;
glm::vec2 lastMousePos;
glm::vec2 mouseDelta;
glm::vec2 move;
int main() {

	
	m_window.InitWindow();
	m_window.CreateWindow(1920,1080);
	m_window.MakeWindowCurrentCtx();
	m_window.ResizeCallBack(ResizeCallBack);
	glewInit();
	glViewport(0, 0, m_window.m_width, m_window.m_height);
	glEnable(GL_DEPTH_TEST);
	
	std::vector<GameEntity*> gameEntites = std::vector<GameEntity*>();
	Shader shader("Shaders/VertOrth.glsl", "Shaders/FragPBR.glsl");
	Shader shaderEnv("Shaders/VertScreen.glsl", "Shaders/FragEnv.glsl");
	Shader shadowShader( "Shaders/VertShadownMap.glsl", "Shaders/GeoShadowMap.glsl","Shaders/FragShadowMap.glsl");
	//std::unique_ptr<Mesh> monkey = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("monke.m"));
	//std::unique_ptr<Mesh> sphere = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("sphere1.m")); 
	//std::unique_ptr<Mesh> quad = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("quad.m")); 
	std::unique_ptr<Mesh> bolb = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("LightBolb.m"));
	//std::unique_ptr<Mesh> closet = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("Closet.m"));

	unsigned int albedoTex = AssetLoader::LoadTexture("rosewood_diff.jpg");
	unsigned int normalTex = AssetLoader::LoadTexture("rosewood_rough.jpg");
	unsigned int metallicTex = AssetLoader::LoadTexture("rosewood_rough.jpg");
	unsigned int roughnessTex = AssetLoader::LoadTexture("rosewood_rough.jpg");

	unsigned int albedoTex1 = AssetLoader::LoadTexture("metal/metal_0002_color_1k.jpg");
	unsigned int normalTex1 = AssetLoader::LoadTexture("metal/metal_0002_normal_opengl_1k.png");
	unsigned int metallicTex1 = AssetLoader::LoadTexture("metal/metal_0002_metallic_1k.jpg");
	unsigned int roughnessTex1 = AssetLoader::LoadTexture("metal/metal_0002_roughness_1k.jpg");
	

	unsigned int albedoTexCloset = AssetLoader::LoadTexture("LowWardrobe_Material_AlbedoTransparency.png");
	unsigned int normalCloset = AssetLoader::LoadTexture("LowWardrobe_Material_Normal.png");
	unsigned int metalicRoughCloset = AssetLoader::LoadTexture("LowWardrobe_Material_MetallicSmoothness.png");

	
	/*GameEntity* MeshObject = new GameEntity(sphere.get(), &shader);
	GameEntity* MeshObject1 = new GameEntity(monkey.get(), &shader);
	GameEntity* MeshObjectCloset = new GameEntity(closet.get(), &shader);*/
	GameEntity* lightMesh = new GameEntity(bolb.get(), &shader);
	//GameEntity* floor = new GameEntity(quad.get(), &shader);
	/*gameEntites.push_back(MeshObject);
	gameEntites.push_back(MeshObject1);
	gameEntites.push_back(floor);
	gameEntites.push_back(MeshObjectCloset);*/
	EnvoirmentMap EnvMap = EnvoirmentMap();
	Camera mainCamera = Camera();
	mainCamera.m_FOV = 80;
	mainCamera.m_position = glm::vec3(0, 0, -3);
	unsigned int envText = AssetLoader::LoadTexture("env4k.hdr");

	EnvMap.Init(envText,&shaderEnv);

	lightMesh->Scale(0.1f, 0.1f, 0.1f);

	/*MeshObject->SetMaterialProperties(albedoTex, normalTex, roughnessTex, roughnessTex);
	MeshObject1->SetMaterialProperties(albedoTex1, normalTex1, metallicTex1, roughnessTex1);
	MeshObjectCloset->SetMaterialProperties(albedoTexCloset, normalCloset, metalicRoughCloset, metalicRoughCloset);
	floor->SetMaterialProperties(albedoTex, normalTex, metallicTex, roughnessTex);*/
	ShadowMap shadowMap = ShadowMap();
	shadowMap.Init();
	/*floor->SetPosition(glm::vec3(0, -2, 0));
	floor->Scale(2, 2, 2);
	MeshObject1->SetPosition(glm::vec3(1,1, -3));
	MeshObject->SetPosition(glm::vec3(-1, -1, -1));*/
    // make sure to initialize matrix to identity matrix first
	
	UIManager::Init(m_window);
	
	std::vector<Light> lights = std::vector<Light>{ Light{1,0.2f,glm::vec3(1,1,1),glm::vec3(0,-2,0)} };

	bool fresnelDebug = false;
	bool geomtryDebug = false;
	bool distrubtionDebug = false;
	for (auto& light : lights) {
		light.InitShadows();
	}
	using ms = std::chrono::duration<float, std::milli>;
	float deltaTime=0.0f;
	float speed = 1;
	std::chrono::high_resolution_clock timer;
	float sensitiviy = 0.01f;
	glfwSetInputMode(m_window.GetWindowID(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	DungeonGenerator dungeon(10,10, 25, gameEntites, &shader);
	while (!m_window.ShouldClose())
	{
		//Add AABB for Oclusion Culling

		ProcessInputs();
		
		auto start = timer.now();
	
		/*for (auto& light : lights) {
			light.ConstructShadowMatrix();
			light.InitRenderShadows(  shadowMap.m_frameBuffer);

			for (int i = 0; i < 6; i++)
			{
				shadowShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", light.shadowTransforms[i]);
			}


			shadowShader.setVec3("lightPos", light.pos);
			shadowShader.setFloat("far_plane", light.shadowFarPlane);
			for (auto& gameEntity : gameEntites)
			{

				gameEntity->DrawShadow(&shadowShader,light);


			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDrawBuffer(GL_FRONT);
			glReadBuffer(GL_FRONT);
		}*/

		
		
		glViewport(0, 0, m_window.m_width, m_window.m_height);
		
		mainCamera.Rotate(-mouseDelta.y * deltaTime * sensitiviy * 0.01f, -mouseDelta.x * sensitiviy * deltaTime * 0.01f, 0);
		mainCamera.Translate((mainCamera.forward * -move.y + mainCamera.right * move.x) * deltaTime * speed*0.01f);
		mainCamera.UpdateVPMat(m_window.m_width, m_window.m_height);
		
		EnvMap.DrawMap(mainCamera.m_projection, mainCamera.m_rotationMat, mainCamera.m_position);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		lightMesh->SetPosition(lights[0].pos);
		lightMesh->Draw(mainCamera.m_projection, mainCamera.m_view, lights, mainCamera.m_position,deltaTime, envText);
		for (size_t i = 0; i < gameEntites.size(); i++)
		{
			gameEntites[i]->Draw(mainCamera.m_projection, mainCamera.m_view, lights, mainCamera.m_position, deltaTime, envText);
		}
		UIManager::BeginDrawUI();
		ImGui::Begin("lights");
		for(int i =0;i<lights.size();i++)
		{ 
			std::string text = "Light " + std::to_string(i);
			ImGui::Text(text.c_str());
			ImGui::DragFloat3(("Light Position" + std::to_string(i)).c_str(), (float*)&lights[i].pos, 0.1f);
			ImGui::DragFloat(("Light intensity" + std::to_string(i)).c_str(), &lights[i].intensity,0.1f);
			ImGui::DragFloat3(("Light Color" + std::to_string(i)).c_str(), (float*)&lights[i].color,0.1f,0,1);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
		}
		ImGui::End();
		ImGui::Begin("Shader Programe");
		if (ImGui::Button("Reload Shaders")) {
			shader= Shader("Shaders/VertOrth.glsl","Shaders/FragSimpleColor.glsl");
			shaderEnv = Shader("Shaders/VertScreen.glsl", "Shaders/FragEnv.glsl");
			shadowShader = Shader("Shaders/VertShadownMap.glsl", "Shaders/GeoShadowMap.glsl", "Shaders/FragShadowMap.glsl");
		}
		if (ImGui::Checkbox("Fresnel Debug", &fresnelDebug))
		{
			////MeshObject->GraphicsDebugFresnel(fresnelDebug);
			//MeshObject1->GraphicsDebugFresnel(fresnelDebug);
		}
		if (ImGui::Checkbox("Geometry Debug", &geomtryDebug))
		{
			//MeshObject->GraphicsDebugGeometry(geomtryDebug);
			//MeshObject1->GraphicsDebugGeometry(geomtryDebug);
		}
		if (ImGui::Checkbox("DistrubtionDebug Debug", &distrubtionDebug))
		{
			//MeshObject->GraphicsDebugDistribution(distrubtionDebug);
			//MeshObject1->GraphicsDebugDistribution(distrubtionDebug);
		}
		ImGui::End();
		ImGui::Begin("Camera");
		ImGui::DragFloat3("Rotation", (float*)&mainCamera.m_rotation, 0.1f);
		ImGui::DragFloat3("Position", (float*)&mainCamera.m_position, 0.1f);
		ImGui::DragFloat("FOV", (float*)&mainCamera.m_FOV, 0.1f);
		ImGui::DragFloat("Sensitivity", &sensitiviy, 0.1f);
		ImGui::DragFloat("Speed", &speed, 0.1f);
	    ImGui::End();
		ImGui::Begin("meshObject");
		//ImGui::DragFloat3("Position" , (float*)&MeshObject->m_position, 0.1f);
		//ImGui::DragFloat3("Rotation", (float*)&MeshObject->m_rotation, 0.1f);
		ImGui::End();
		ImGui::Begin("meshObject1");
		//ImGui::DragFloat3("Position", (float*)&MeshObject1->m_position, 0.1f);
		//ImGui::DragFloat3("Rotation", (float*)&MeshObject1->m_rotation, 0.1f);
		ImGui::End();
		ImGui::Begin("meshObject Closet");
		//ImGui::DragFloat3("Position", (float*)&MeshObjectCloset->m_position, 0.1f);
		//ImGui::DragFloat3("Rotation", (float*)&MeshObjectCloset->m_rotation, 0.1f);
		ImGui::End();
		UIManager::EndDrawUI();
		m_window.SwapBuffer();
		glfwPollEvents();
		auto stop = timer.now();
		deltaTime = std::chrono::duration_cast<ms>(stop - start).count();
	}
	return 0;

}

void ProcessInputs() 
{
	mouseDelta = glm::vec2(0, 0);
	move = glm::vec2(0, 0);
	double mousePosx = 0;
	double mousePosy = 0;
	if (glfwGetInputMode(m_window.GetWindowID(), GLFW_CURSOR) != GLFW_CURSOR_NORMAL)
	{
	
		glfwGetCursorPos(m_window.GetWindowID(), &(mousePosx), &mousePosy);

		mouseDelta = lastMousePos - glm::vec2(mousePosx, mousePosy);

		lastMousePos = glm::vec2(mousePosx, mousePosy);
		
	}
	int state = glfwGetKey(m_window.GetWindowID(), GLFW_KEY_ESCAPE);
	if (state == GLFW_PRESS)
	{
		glfwSetInputMode(m_window.GetWindowID(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	state = glfwGetKey(m_window.GetWindowID(), GLFW_KEY_ENTER);
	if (state == GLFW_PRESS)
	{
		glfwGetCursorPos(m_window.GetWindowID(), &(mousePosx), &mousePosy);
		lastMousePos = glm::vec2(mousePosx, mousePosy);
		glfwSetInputMode(m_window.GetWindowID(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	state = glfwGetKey(m_window.GetWindowID(), GLFW_KEY_W);
	if (state == GLFW_PRESS)
	{
		move.y = 1;
	}
	state = glfwGetKey(m_window.GetWindowID(), GLFW_KEY_S);
	if (state == GLFW_PRESS)
	{
		move.y = -1;
	}
	state = glfwGetKey(m_window.GetWindowID(), GLFW_KEY_D);
	if (state == GLFW_PRESS)
	{
		move.x = 1;
	}
	state = glfwGetKey(m_window.GetWindowID(), GLFW_KEY_A);
	if (state == GLFW_PRESS)
	{
		move.x = -1;
	}

}
void ResizeCallBack(GLFWwindow* window, int w, int h) 
{
	m_window.m_width = w;
	m_window.m_height = h;
	glViewport(0, 0,w,h);
}

