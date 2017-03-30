#pragma once

inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 q)
{
	XMFLOAT3 euler;

	euler.y = atan2f(2.f * q.x * q.w + 2.f * q.y * q.z, 1.f - 2.f * ((q.z*q.z) + (q.w*q.w)));     // Yaw 
	euler.x = asinf(2.f * (q.x * q.z - q.w * q.y));                             // Pitch 
	euler.z = atan2f(2.f * q.x * q.y + 2.f * q.z * q.w, 1.f - 2.f * ((q.y*q.y) + (q.z*q.z)));      // Roll 

	return euler;
}

inline bool XMFloat4Equals(const XMFLOAT4& a, const XMFLOAT4& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.w);
}

inline bool XMFloat3Equals(const XMFLOAT3& a, const XMFLOAT3& b)
{
	return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool XMFloat2Equals(const XMFLOAT2& a, const XMFLOAT2& b)
{
	return (a.x == b.x) && (a.y == b.y);
}

inline float randF(float min, float max)
{
	float random = ((float) rand()) / (float) RAND_MAX;
	float diff = max - min;
	float r = random * diff;
	return min + r;
}

XMFLOAT3 operator+(const XMFLOAT3& lhs, const XMFLOAT3&rhs);

XMFLOAT3 operator-(const XMFLOAT3& lhs, const XMFLOAT3&rhs);