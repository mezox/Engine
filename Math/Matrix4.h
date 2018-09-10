#pragma once

#include "Math/Vector3.h"

namespace Engine::Math
{
	class Matrix4
	{
	public:
		Matrix4();


		inline float& operator()(int32_t row, int32_t col) { return mData[(row << 2) + col]; }
		inline const float& operator()(int32_t row, int32_t col) const { return mData[(row << 2) + col]; }

		void SetIdentity();
		void SetTranslation(const Vector3<float>& translation);
		void SetScale(const Vector3<float>& scale);
		void SetRotation(const Vector3<float>& rotation);

		const float* GetData() const
		{
			return &mData[0];
		}
		
	private:
		float mData[16];
	};
}