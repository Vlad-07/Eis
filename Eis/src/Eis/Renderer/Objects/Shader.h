#pragma once

namespace Eis
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static Shader* Create(std::string& vsSrc, std::string& fsSrc);
	};
}