#include "Eispch.h"
#include "OpenGLRendererAPI.h"

#include "Eis/Rendering/Objects/VertexArray.h"
#include "Eis/Assets/AssetManager.h"

#include <glad/glad.h>


namespace Eis
{
	void OpenGLRendererAPI::Init()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


	void OpenGLRendererAPI::DrawSubMesh(const SubMesh& submesh)
	{
		glDrawElements(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(submesh.FirstIndex * sizeof(uint32_t)));
	}
}