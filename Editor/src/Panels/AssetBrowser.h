#pragma once

#include <filesystem>

#include <Eis/Rendering/Objects/Texture.h>


namespace Eis
{
	class AssetBrowser
	{
	public:
		AssetBrowser();
		~AssetBrowser() = default;


		void OnImGuiRender();

	private:
		void ChangeDir(const std::filesystem::path& path);

		void OpenFile(const std::filesystem::path& path);

	private:
		std::filesystem::path m_CurrentPath;

		Ref<Texture2D> m_FolderTex, m_FileTex;

		std::vector<std::filesystem::directory_entry> m_Files;
	};
}