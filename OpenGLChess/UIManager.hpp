

class UIManager
{
public :

	static void BeginDrawUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	

		ImGui::Begin("Hello, world!");

		ImGui::Text("This is some useful text.");

		
	}

	static void EndDrawUI() {
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	

};