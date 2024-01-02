#pragma once

#include <glm/glm.hpp>

namespace GameTheory
{
	enum Strategy
	{
		None = 0, A, B
	};

	class RewardMatrix
	{
	public:
		RewardMatrix() = default;
		RewardMatrix(glm::mat2 mat, float defaultReward) : m_Mat(mat), m_DefaultReward(defaultReward) {}
		~RewardMatrix() = default;

		float GetReward(Strategy self, Strategy opponent)
		{
			if (self == None || opponent == None)
				return m_DefaultReward;

			return m_Mat[(int)self - 1][(int)opponent - 1];
		}

		float GetDefaultReward() const { return m_DefaultReward; }

		void LoadMatrix(glm::mat2 mat) { m_Mat = mat; };
		glm::mat2& GetMatrix() { return m_Mat; }
		const glm::mat2& GetMatrix() const { return m_Mat; }

	private:
		glm::mat2 m_Mat;
		float m_DefaultReward;
	};
}