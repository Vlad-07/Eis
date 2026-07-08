#include "AssetBrowser.h"

#include "Eis/Project/Project.h"
#include "Eis/Assets/Importers/TextureImporter.h"

#include <imgui.h>



namespace Eis
{
	AssetBrowser::AssetBrowser()
		: m_BasePath{ Project::GetAssetDir() }, m_CurrentPath{ Project::GetAssetDir() }
	{
		m_DirectoryTex = TextureImporter::LoadTexture2D("resources/icons/folder.png");
		m_FileTex = TextureImporter::LoadTexture2D("resources/icons/file.png");

		ChangeDir(Project::GetAssetDir());
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

		if (m_CurrentPath != Project::GetAssetDir())
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
					const bool isDir = dirEntry.IsDir;
					const AssetType type = dirEntry.Type;
					const std::string fileName = path.filename().string();

					// Hide unimported files
					if (!m_ShowAllFiles && !isDir && type == AssetType::None)
						continue;

					ImGui::TableNextColumn();
					ImGui::PushID(fileName.c_str());

					// Icon
					ImGui::ImageButton(fileName.c_str(),
						SelectIcon(dirEntry)->GetRendererId(),
						{ m_ItemSize, m_ItemSize }, { 0,1 }, { 1,0 });

					// Options
					if (ImGui::BeginPopupContextItem("ItemOptions"))
					{
						if (!isDir)
						{
							if (type == AssetType::None)
							{
								if (ImGui::MenuItem("Import"))
								{
									dirEntry.Handle = Project::GetEditorAssetManager()->ImportAsset(path);
									RefreshFileStatus(dirEntry);
								}
							}
							else
							{
								if (ImGui::MenuItem("Remove"))
								{
									Delete(it);
									ImGui::EndPopup();
									ImGui::PopID();
									break;
								}
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

					// Name
					ImGui::TextWrapped("%s", fileName.c_str());
					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{}// TODO: rename
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
		for (const auto& dirIt : std::filesystem::directory_iterator{ m_CurrentPath })
		{
			DirEntry& entry = m_Files.emplace_back(dirIt.path(), dirIt.is_directory());
			RefreshFileStatus(entry);
		}
	}

	void AssetBrowser::RefreshFileStatus(DirEntry& dirEntry)
	{
		if (dirEntry.IsDir)
			return;

		if (!dirEntry.Handle)
			dirEntry.Handle = Project::GetEditorAssetManager()->GetAsset(dirEntry.Path);
		dirEntry.Type = Project::GetEditorAssetManager()->GetAssetType(dirEntry.Handle);
	}

	void AssetBrowser::OpenFile(const std::filesystem::path& path)
	{
		if (path.extension() == ".eis")
		{
			// TODO: open scene
		}
		// and edit selected asset etc...
		// kind of requires an editor event system
	}


	void AssetBrowser::CreateDir(const std::filesystem::path& dir, std::string_view name)
	{
		EIS_CORE_ASSERT(std::filesystem::is_directory(dir));

		std::filesystem::create_directory(m_Files.back().Path);
		m_Files.emplace_back(dir / name, true);
	}

	void AssetBrowser::CreateFile(const std::filesystem::path& dir, AssetType type)
	{
		EIS_CORE_ASSERT(std::filesystem::is_directory(dir));

		std::string filename = AssetTypeToString(type) + AssetTypeToExtension(type);

		std::filesystem::path path = dir / filename;
		if (std::filesystem::exists(path))
		{
			// TODO: add (1), (2), ... to existing filenames
		}

		std::ofstream out{ path, std::ios::app }; out.close();

		AssetHandle handle = Project::GetEditorAssetManager()->ImportAsset(path);
		DirEntry& entry = m_Files.emplace_back(path, false);
		entry.Type = type;
		entry.Handle = handle;
	}

	void AssetBrowser::Delete(std::vector<DirEntry>::iterator it)
	{
		Project::GetEditorAssetManager()->RemoveAsset(it->Handle);
		m_Files.erase(it);
	}


	const Ref<Texture2D>& AssetBrowser::SelectIcon(const DirEntry& dirEntry)
	{
		if (dirEntry.IsDir)
			return m_DirectoryTex;

		switch (dirEntry.Type)
		{
			case AssetType::Texture2D:
				// TODO: asset type icons
				break;
		}

		return m_FileTex;
	}
}