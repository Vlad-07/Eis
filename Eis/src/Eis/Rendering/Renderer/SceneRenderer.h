#pragma once

#include "Eis/Rendering/Objects/Mesh.h"


namespace Eis
{
	class Camera;
	class EditorCamera2;

	class SceneRenderer
	{
	public:
		static void BeginScene(const Camera& cam, const glm::mat4& transform);
		static void BeginScene(const EditorCamera2& cam);
		static void EndScene();

		static void DrawMesh(const glm::mat4& transform, const Ref<StaticMesh>& mesh);

	private:
		friend class Application;
		static void Init();


		static void Pass();


		static void CalcFrustumPlanes(const glm::mat4& viewProj);
		static bool IsInsideFrustum(const StaticMesh& mesh);
	};
}