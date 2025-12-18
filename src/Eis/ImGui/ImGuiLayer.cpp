#include "Eispch.h"
#include "ImGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_opengl3_loader.h>
#include <implot.h>

#include <GLFW/glfw3.h>

#include "Eis/Core/Application.h"

#ifdef EIS_PLATFORM_WEB
	#include <fstream>
#endif


namespace Eis
{
	void ImGuiLayer::Attach()
	{
		EIS_PROFILE_FUNCTION();		

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	#ifdef EIS_IMGUI_VIEWPORTS
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigViewportsNoTaskBarIcon = true;
	#endif

		// HACK: these options actually require viewports
		// might cause problems with new imgui versions but work fine-ish for now
		io.ConfigDpiScaleFonts = true;
		io.ConfigDpiScaleViewports = true;

	#ifdef EIS_PLATFORM_WEB
		io.IniFilename = "assets/imgui.ini";
	#endif

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		GLFWwindow* window = static_cast<GLFWwindow*>(Application::GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void ImGuiLayer::Detach()
	{
		EIS_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}


	void ImGuiLayer::Begin()
	{
		EIS_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		EIS_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		const Window& window = Application::GetWindow();
		io.DisplaySize = ImVec2(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}