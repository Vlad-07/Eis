#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Eis
{
	class Shader
	{
	private:
		uint32_t m_RendererId;

	public:
		Shader(std::string& vsSrc, std::string& fsSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}