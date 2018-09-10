#include "Matrix4.h"

using namespace Engine::Math;

Matrix4::Matrix4()
{
	SetIdentity();
}

void Matrix4::SetIdentity()
{
	std::memset(&mData, 0.0f, 16 * sizeof(float));
	(*this)(0, 0) = 1.0f;
	(*this)(1, 1) = 1.0f;
	(*this)(2, 2) = 1.0f;
	(*this)(3, 3) = 1.0f;
}

void Matrix4::SetTranslation(const Vector3<float>& translation)
{
	(*this)(3, 0) = translation.x;
	(*this)(3, 1) = translation.y;
	(*this)(3, 2) = translation.z;
}

void Matrix4::SetScale(const Vector3<float>& scale)
{
	(*this)(0, 0) = scale.x;
	(*this)(1, 1) = scale.y;
	(*this)(2, 2) = scale.z;
}

void Matrix4::SetRotation(const Vector3<float>& rotation)
{
}

