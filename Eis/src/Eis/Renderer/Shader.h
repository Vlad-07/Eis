#pragma once

#include <string>

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


	};
}