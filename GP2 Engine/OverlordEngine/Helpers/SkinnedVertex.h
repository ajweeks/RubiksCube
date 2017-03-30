#pragma once

#include "VertexHelper.h"
#include <DirectXMath.h>

struct SkinnedVertex
{
	SkinnedVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 normal, DirectX::XMFLOAT4 color,
		float blend) :
		TransformedVertex(position, normal, color),
		OriginalVertex(position, normal, color),
		Blend(blend)
	{}

	VertexPosNormCol TransformedVertex;
	VertexPosNormCol OriginalVertex;
	float Blend;
};
