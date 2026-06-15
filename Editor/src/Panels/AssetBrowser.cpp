#include "AssetBrowser.h"

#include "Eis/Project/Project.h"
#include "Eis/Assets/Importers.h"

#include <imgui.h>



namespace Eis
{
	AssetBrowser::AssetBrowser()
		: m_BasePath{ Project::GetAssetsDir() }, m_CurrentPath{ Project::GetAssetsDir() }
	{
		m_DirectoryTex = TextureImporter::LoadTexture2D("resources/icons/folder.png");
		m_FileTex = TextureImporter::LoadTexture2D("resources/icons/file.png");

		ChangeDir(Project::GetAssetsDir());
	}


	void AssetBrowser::OnImGuiRender()
	{
		ImGui::Begin("Asset Browser", 0, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			// TODO: settings and menu bar
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::SliderFloat("Item size", &m_ItemSize, 32.0f, 200.0f, "%.1f");
				ImGui::Checkbox("Show all", &m_ShowAllFiles);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if (m_CurrentPath != Project::GetAssetsDir())
		{
			if (ImGui::Button("<-"))
				ChangeDir(m_CurrentPath.parent_path());
		}


		// TODO: tree view


		// Items
		{
			const float padding = ImGui::GetStyle().CellPadding.x * 2.0f;
			int columns = (int)glm::floor(ImGui::GetContentRegionAvail().x / (m_ItemSize + padding));
			if (columns < 1) columns = 1;
			if (ImGui::BeginTable("Files", columns, ImGuiTableFlags_SizingStretchSame))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, { 0, 0, 0, 0 });
				for (auto it{ m_Files.begin() }; it != m_Files.end(); it++)
				{
					DirEntry& dirEntry = *it;

					const auto& path = dirEntry.Path;
					const auto& relativePath = std::filesystem::relative(dirEntry.Path, Project::GetAssetsDir());
					const bool isDir = dirEntry.IsDir;
					const AssetType type = dirEntry.AssetType;
					const std::string fileName = path.filename().string();

					// Hide unimported files
					if (!m_ShowAllFiles && !isDir && type == AssetType::None)
						continue;

					ImGui::TableNextColumn();
					ImGui::PushID(fileName.c_str());

					ImGui::ImageButton(fileName.c_str(),
						SelectIcon(dirEntry)->GetRendererId(),
						{ m_ItemSize, m_ItemSize }, { 0,1 }, { 1,0 });

					// Options
					if (ImGui::BeginPopupContextItem("ItemOptions"))
					{
						// Common

						if (ImGui::MenuItem("Delete"))
						{
							Delete(it);
							ImGui::EndPopup();
							ImGui::PopID();
							break;
						}

						// Importing

						if (!isDir && type == AssetType::None)
						{
							if (ImGui::MenuItem("Import"))
							{
								Project::GetEditorAssetManager()->ImportAsset(relativePath);
								RefreshFileStatus(dirEntry);
							}
						}

						ImGui::EndPopup();
					}

					// Drag
					if (type != AssetType::None && ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("ASSET_DRAG", &dirEntry.Handle, sizeof(AssetHandle), ImGuiCond_Once);
						ImGui::EndDragDropSource();
					}

					// Change dir / Open file
					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							if (isDir)
							{
								ChangeDir(path); // maybe deffer
								ImGui::PopID();
								break; // to not mess up the loop
							}
							else
								OpenFile(path);
						}
					}

					ImGui::TextWrapped("%s", fileName.c_str());
					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{}// rename
					}

					ImGui::PopID();
				}

				ImGui::EndTable();
				ImGui::PopStyleColor();
			}

			// RMB between items
			if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("Folder"))
						CreateDir(m_CurrentPath);

					if (ImGui::MenuItem("Material"))
						CreateFile(m_CurrentPath, AssetType::Material);

					ImGui::EndMenu();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}


	void AssetBrowser::ChangeDir(const std::filesystem::path& path)
	{
		m_CurrentPath = path;

		m_Files.clear();
		for (auto& p : std::filesystem::directory_iterator{ m_CurrentPath })
			m_Files.emplace_back(p.path(), p.is_directory());

		RefreshAllFileAssetStatus();
	}

	void AssetBrowser::RefreshAllFileAssetStatus()
	{
		// feels slow...
		for (auto& dirEntry : m_Files)
		{
			if (dirEntry.IsDir)
				continue;

			RefreshFileStatus(dirEntry);
		}
	}

	void AssetBrowser::RefreshFileStatus(DirEntry& dirEntry)
	{
		if (dirEntry.IsDir)
			return;

		const std::filesystem::path& relativePath = std::filesystem::relative(dirEntry.Path, Project::GetAssetsDir());

		dirEntry.Handle = Project::GetEditorAssetManager()->GetAssetByPath(relativePath);
		dirEntry.AssetType = Project::GetEditorAssetManager()->GetAssetType(dirEntry.Handle);
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


	void AssetBrowser::CreateDir(const std::filesystem::path& dir, std::string_view name)
	{
		EIS_CORE_ASSERT(std::filesystem::is_directory(dir));

		m_Files.emplace_back(dir / name, true);
		std::filesystem::create_directory(m_Files.back().Path);
	}

	void AssetBrowser::CreateFile(const std::filesystem::path& dir, AssetType type)
	{
		EIS_CORE_ASSERT(std::filesystem::is_directory(dir));

		std::string stem = AssetTypeToString(type) + AssetTypeToExtension(type);

		std::filesystem::path path = dir / stem;
		if (std::filesystem::exists(path))
		{
			// TODO:
		}

		std::ofstream out{ path };
	}

	void AssetBrowser::Delete(std::vector<DirEntry>::iterator it)
	{
		std::filesystem::remove_all(it->Path);
		m_Files.erase(it);
	}


	const Ref<Texture2D>& AssetBrowser::SelectIcon(const DirEntry& dirEntry)
	{
		if (dirEntry.IsDir)
			return m_DirectoryTex;

		switch (dirEntry.AssetType)
		{
			case AssetType::Texture2D:
				// TODO: asset type icons
				break;
		}

		return m_FileTex;
	}
}