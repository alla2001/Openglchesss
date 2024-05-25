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

#include "UIManager.hpp"
#include "EnvoirmentMap.hpp"

void ResizeCallBack(GLFWwindow* window, int w, int h);

Window m_window;
int main() {

	
	m_window.InitWindow();
	m_window.CreateWindow(1920,1080);
	m_window.MakeWindowCurrentCtx();
	m_window.ResizeCallBack(ResizeCallBack);
	glewInit();
	glViewport(0, 0, m_window.m_width, m_window.m_height);
	glEnable(GL_DEPTH_TEST);
	const char* glsl_version = "#version 150";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(m_window.GetWindowID(), true);

	ImGui_ImplOpenGL3_Init(glsl_version);

	Shader shader("VertOrth.glsl", "FragSimpleColor.glsl");
	std::unique_ptr<Mesh> monkey = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("monke.m"));
	std::unique_ptr<Mesh> quad = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("sphere1.m")); std::unique_ptr<Mesh> bolb = std::unique_ptr<Mesh>(AssetLoader::LoadMesh("LightBolb.m"));


	unsigned int albedoTex = AssetLoader::LoadTexture("rosewood_diff.jpg");
	unsigned int normalTex = AssetLoader::LoadTexture("rosewood_rough.jpg");
	unsigned int metallicTex = AssetLoader::LoadTexture("rosewood_rough.jpg");
	unsigned int roughnessTex = AssetLoader::LoadTexture("rosewood_rough.jpg");

	unsigned int albedoTex1 = AssetLoader::LoadTexture("metal/metal_0002_color_1k.jpg");
	unsigned int normalTex1 = AssetLoader::LoadTexture("rosewood_rough.jpg");
	unsigned int metallicTex1 = AssetLoader::LoadTexture("metal/metal_0002_metallic_1k.jpg");
	unsigned int roughnessTex1 = AssetLoader::LoadTexture("metal/metal_0002_roughness_1k.jpg");
	
	GameEntity* MeshObject = new GameEntity(quad.get(), &shader);
	GameEntity* MeshObject1 = new GameEntity(monkey.get(), &shader);
	GameEntity* lightMesh = new GameEntity(bolb.get(), &shader);
	Shader shaderEnv("VertScreen.glsl", "FragEnv.glsl");
	EnvoirmentMap EnvMap = EnvoirmentMap();
	unsigned int envText = AssetLoader::LoadTexture("metal/metal_0002_color_1k.jpg");
	EnvMap.Init(envText,&shaderEnv);
	lightMesh->Scale(0.1f, 0.1f, 0.1f);
	MeshObject->SetMaterialProperties(albedoTex, normalTex, metallicTex, roughnessTex);
	MeshObject1->SetMaterialProperties(albedoTex1, normalTex1, metallicTex1, roughnessTex1);
	glm::vec3 camPos;
	MeshObject1->SetPosition(glm::vec3(1,1, -3));
	MeshObject->SetPosition(glm::vec3(-1, -1, -1));
	glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 projection = glm::mat4(1.0f);
	camPos = glm::vec3(0.0f, 0.0f, -3.0f);
	view = glm::translate(view,camPos);
	std::vector<Light> lights = std::vector<Light>{ Light{1,0.2f,glm::vec3(1,1,1),glm::vec3(0,-2,0)} };

	bool fresnelDebug = false;
	bool geomtryDebug = false;
	bool distrubtionDebug = false;
	while (!m_window.ShouldClose())
	{
		projection = glm::perspective(glm::radians(45.0f), (float)m_window.m_width / (float)m_window.m_height, 0.1f, 100.0f);
		glClearColor(0.7f, 0.8f, 0.7f, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		lightMesh->SetPosition(lights[0].pos);
		lightMesh->Draw(projection, view, lights, camPos);
		MeshObject1->Draw(projection, view, lights, camPos);
		MeshObject->Draw(projection, view, lights, camPos);
		UIManager::BeginDrawUI();
		
		for(int i =0;i<lights.size();i++)
		{ 
			
			ImGui::DragFloat3(("Light Position" + std::to_string(i)).c_str(), (float*)&lights[i].pos, 0.1f);
			ImGui::DragFloat(("Light intensity" + std::to_string(i)).c_str(), &lights[i].intensity,0.1f);
			ImGui::DragFloat3(("Light Color" + std::to_string(i)).c_str(), (float*)&lights[i].color,0.1f,0,1);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();
		}

		ImGui::Begin("Shader Programe");
		if (ImGui::Button("Reload Shaders")) {
			shader= Shader("VertOrth.glsl", "FragSimpleColor.glsl");
		}
		if (ImGui::Checkbox("Fresnel Debug", &fresnelDebug))
		{
			MeshObject->GraphicsDebugFresnel(fresnelDebug);
			MeshObject1->GraphicsDebugFresnel(fresnelDebug);
		}
		if (ImGui::Checkbox("Geometry Debug", &geomtryDebug))
		{
			MeshObject->GraphicsDebugGeometry(geomtryDebug);
			MeshObject1->GraphicsDebugGeometry(geomtryDebug);
		}
		if (ImGui::Checkbox("DistrubtionDebug Debug", &distrubtionDebug))
		{
			MeshObject->GraphicsDebugDistribution(distrubtionDebug);
			MeshObject1->GraphicsDebugDistribution(distrubtionDebug);
		}
		ImGui::End();
		ImGui::Begin("meshObject");
		ImGui::DragFloat3("Position" , (float*)&MeshObject->m_position, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&MeshObject->m_rotation, 0.1f);
		ImGui::End();
		ImGui::Begin("meshObject1");
		ImGui::DragFloat3("Position", (float*)&MeshObject1->m_position, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&MeshObject1->m_rotation, 0.1f);
		ImGui::End();
		UIManager::EndDrawUI();
		m_window.SwapBuffer();
		glfwPollEvents();
		
	}
	return 0;

}

void ResizeCallBack(GLFWwindow* window, int w, int h) 
{
	m_window.m_width = w;
	m_window.m_height = h;
	glViewport(0, 0,w,h);
}


