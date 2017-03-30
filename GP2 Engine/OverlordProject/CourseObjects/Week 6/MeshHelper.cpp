
#include "stdafx.h"

#include "MeshHelper.h"
#include "Helpers\MathHelper.h"

void PushBackBox(std::vector<SkinnedVertex>& verts, const XMFLOAT3 & pos, const XMFLOAT3 & offset, const XMFLOAT4 & col, bool firstBone)
{
	verts.reserve(verts.size() + 24);

	const XMFLOAT3 points[] =
	{
		XMFLOAT3(-offset.x, offset.y, -offset.z),	// 0
		XMFLOAT3(offset.x, offset.y, -offset.z),	// 1
		XMFLOAT3(offset.x, -offset.y, -offset.z),	// 2
		XMFLOAT3(-offset.x , -offset.y, -offset.z),	// 3

		XMFLOAT3(offset.x, offset.y, offset.z),		// 4
		XMFLOAT3(-offset.x, offset.y, offset.z),	// 5
		XMFLOAT3(-offset.x, -offset.y, offset.z),	// 6
		XMFLOAT3(offset.x, -offset.y, offset.z)		// 7
	};

	float index = firstBone ? 0.0f : 1.0f;

	// FRONT
	XMFLOAT3 norm = XMFLOAT3(0, 0, -1);
	verts.push_back(SkinnedVertex(pos + points[0], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[1], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[2], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[3], norm, col, index * 0.5f));
	// BACK
	norm = XMFLOAT3(0, 0, 1);
	verts.push_back(SkinnedVertex(pos + points[4], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[5], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[6], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[7], norm, col, 0.5f + index * 0.5f));
	// TOP
	norm = XMFLOAT3(0, 1, 0);
	verts.push_back(SkinnedVertex(pos + points[5], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[4], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[1], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[0], norm, col, index * 0.5f));
	// BOTTOM
	norm = XMFLOAT3(0, -1, 0);
	verts.push_back(SkinnedVertex(pos + points[3], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[2], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[7], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[6], norm, col, index * 0.5f));
	// LEFT
	norm = XMFLOAT3(-1, 0, 0);
	verts.push_back(SkinnedVertex(pos + points[6], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[0], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[3], norm, col, index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[6], norm, col, index * 0.5f));
	// RIGHT
	norm = XMFLOAT3(1, 0, 0);
	verts.push_back(SkinnedVertex(pos + points[1], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[4], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[7], norm, col, 0.5f + index * 0.5f));
	verts.push_back(SkinnedVertex(pos + points[2], norm, col, 0.5f + index * 0.5f));
}
