

class UIManager
{
public :


	static void Init(Window window) {
		const char* glsl_version = "#version 150";
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window.GetWindowID(), true);

		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	static void BeginDrawUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	

	

		
	}

	static void EndDrawUI() {
	

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	

};