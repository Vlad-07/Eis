#include "Eispch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>


namespace Eis
{
	void OpenGLRendererAPI::Init()
	{
		EIS_PROFILE_RENDERER_FUNCTION();

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
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

	void OpenGLRendererAPI::DrawIndex(const Ref<VertexArray>& va, uint32_t indexCount)
	{
		va->Bind();
		uint32_t count = indexCount ? indexCount : va->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& va, uint32_t vertexCount)
	{
		va->Bind();
		uint32_t count = vertexCount ? vertexCount : va->GetIndexBuffer()->GetCount();
		glDrawArrays(GL_LINES, 0, count);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGLRendererAPI::Enable(uint32_t code)
	{
		glEnable(code);
	}

	void OpenGLRendererAPI::Disable(uint32_t code)
	{
		glDisable(code);
	}
}