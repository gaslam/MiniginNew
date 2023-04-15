#include "ImGuiTutorial.h"
#include "imgui.h"

void dae::ImGuiTutorial::Render() const
{
	ImGui::Begin("Tutorial", nullptr, ImGuiWindowFlags_None);

	ImGui::Text("On start:");
	ImGui::NewLine();
	ImGui::Text("Achievements are cleared automatically upon startup");
	ImGui::NewLine();

	ImGui::Text("Controls:");
	ImGui::NewLine();

	ImGui::Text("Keyboard(QWERTY)");

	ImGui::NewLine();
	ImGui::Text("Player 1");
	ImGui::Text("Increase score: W");
	ImGui::Text("Decrease lives: S");
	ImGui::NewLine();
	ImGui::Text("Player 2");
	ImGui::Text("Increase score: Arrow up");
	ImGui::Text("Decrease lives: Arrow down");

	ImGui::NewLine();
	ImGui::NewLine();
	ImGui::Text("Controller");
	ImGui::Text("Increase score: A");
	ImGui::Text("Decrease lives: B");
	ImGui::End();

}
