#pragma once

#include "Eis/Assets/Asset.h"

#include <filesystem>


namespace Eis
{
	class Texture2D;

	class AssetBrowser
	{
	public:
		AssetBrowser();
		~AssetBrowser() = default;


		void OnImGuiRender();

	private:
		struct DirEntry
		{
			std::filesystem::path Path;
			bool IsDir{ false };
			AssetHandle Handle{ 0 };
			AssetType AssetType = AssetType::None;

			DirEntry(const std::filesystem::path& path, bool isDir)
				: Path{ path }, IsDir{ isDir } {}
		};


		void ChangeDir(const std::filesystem::path& path);
		void RefreshAllFileAssetStatus();
		void RefreshFileStatus(DirEntry& path);

		void OpenFile(const std::filesystem::path& path);


		void CreateDir(const std::filesystem::path& dir, std::string_view name = "New folder");
		void CreateFile(const std::filesystem::path& dir, AssetType type);
		void Delete(std::vector<DirEntry>::iterator it);


		const Ref<Texture2D>& SelectIcon(const DirEntry& dirEntry);

	private:
		std::filesystem::path m_BasePath;
		std::filesystem::path m_CurrentPath;

		Ref<Texture2D> m_DirectoryTex, m_FileTex;


		std::vector<DirEntry> m_Files;


		bool m_ShowAllFiles{ true };
		// TODO: filters for asset types
		float m_ItemSize{ 80.0f };
	};
}