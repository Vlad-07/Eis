#pragma once


namespace Eis
{
	class Camera;
	class EditorCamera2;
	class StaticMesh;

	class SceneRenderer
	{
	public:
		static void BeginScene(const Camera& cam, const glm::mat4& transform);
		static void BeginScene(const EditorCamera2& cam);
		static void EndScene();

		static void DrawMesh(const glm::mat4& transform, const Ref<StaticMesh>& mesh);

	private:
		static void Init();

		static void Pass();

		friend class Application;
	};
}