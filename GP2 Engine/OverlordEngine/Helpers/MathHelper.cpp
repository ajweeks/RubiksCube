#include "stdafx.h"
#include "MathHelper.h"

//Prevents Warning LNK4221 > Caused by empty source files in a Library
//__declspec(dllexport) void getRidOfLNK4221_MathHelper() {}

XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3&rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}


XMFLOAT3 operator-(const XMFLOAT3& lhs, const XMFLOAT3&rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}
