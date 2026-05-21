#pragma once

#include <filesystem>

#include "Eis/Core/Core.h"
#include "Scene.h"


namespace Eis
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);
		~SceneSerializer() = default;

		void Serialize(const std::filesystem::path& path);
		void SerializeRuntime(const std::filesystem::path& path);

		bool Deserialize(const std::filesystem::path& path);
		bool DeserializeRuntime(const std::filesystem::path& path);

	private:
		Ref<Scene> m_Scene;
	};
}