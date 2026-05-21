#include "AssetBrowser.h"
#include <imgui.h>



namespace Eis
{
	// replace with project stuff...
	static const std::filesystem::path c_ProjectAssets{ "assets" };


	AssetBrowser::AssetBrowser()
	{
		m_FolderTex = Texture2D::Create("resources/icons/folder.png");
		m_FileTex = Texture2D::Create("resources/icons/file.png");

		ChangeDir(c_ProjectAssets);
	}


	void AssetBrowser::OnImGuiRender()
	{
		ImGui::Begin("Asset Browser", 0, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			// TODO: settings and menu bar
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (m_CurrentPath != c_ProjectAssets)
		{
			if (ImGui::Button("<-"))
				ChangeDir(m_CurrentPath.parent_path());
		}


		// TODO: tree view


		// Items
		{
			const bool doubleClick = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

			static float itemSize = 72.0f;
			//	ImGui::SliderFloat("Item Size", &itemSize, 32.0f, 256.0f);

			const float padding = ImGui::GetStyle().CellPadding.x * 2.0f;
			int columns = (int)glm::floor(ImGui::GetContentRegionAvail().x / (itemSize + padding));
			if (columns < 1) columns = 1;
			if (ImGui::BeginTable("Files", columns, ImGuiTableFlags_SizingStretchSame))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
				for (auto& dirEntry : m_Files)
				{
					const auto& path = dirEntry.path();
					//auto relativePath = std::filesystem::relative(path, c_ProjectAssets);
					const std::string fileName = path.filename().string();
					const bool isDir = dirEntry.is_directory();


					ImGui::TableNextColumn();


					ImGui::ImageButton(fileName.c_str(),
						isDir ? m_FolderTex->GetRendererId() : m_FileTex->GetRendererId(),
						{ itemSize, itemSize }, { 0,1 }, { 1,0 });

					if (path.extension().string() == ".eis")
					{
						if (ImGui::BeginDragDropSource())
						{
							ImGui::SetDragDropPayload("ASSET_DRAG", path.string().c_str(), path.string().size(), ImGuiCond_Once);
							ImGui::EndDragDropSource();
						}
					}

					if (doubleClick && ImGui::IsItemHovered())
					{
						if (isDir)
						{
							ChangeDir(path); // maybe deffer
							break; // to not mess up the loop
						}
						else
							OpenFile(path);
					}


					ImGui::TextWrapped("%s", fileName.c_str());
				}

				ImGui::EndTable();
				ImGui::PopStyleColor();
			}
		}

		ImGui::End();
	}


	void AssetBrowser::ChangeDir(const std::filesystem::path& path)
	{
		m_CurrentPath = path;

		m_Files.clear();
		for (auto& p : std::filesystem::directory_iterator{ m_CurrentPath })
			m_Files.push_back(p);
	}

	void AssetBrowser::OpenFile(const std::filesystem::path& path)
	{
		if (path.extension() == ".eis")
		{
			// TODO: open scene
		}
		// and edit selected asset etc...
		// kind of requires an asset manager or editor event system
	}
}