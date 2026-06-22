#pragma once

#include "Eis/Rendering/Objects/Camera.h"
#include "Eis/Rendering/Objects/EditorCamera2.h"


namespace Eis
{
	class Mesh;

	class SceneRenderer
	{
	public:
		static void BeginScene(const Camera& cam, const glm::mat4& transform);
		static void BeginScene(const EditorCamera2& cam);
		static void EndScene();

		static void DrawMesh(const glm::mat4& transform, const Ref<Mesh>& mesh);

	private:
		static void Init();

		static void Pass();

		friend class Application;
	};
}