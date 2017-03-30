#pragma once

#include "Helpers\SkinnedVertex.h"

#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

void PushBackBox(std::vector<SkinnedVertex>& verts, const XMFLOAT3& pos, const XMFLOAT3& offset, const XMFLOAT4& col, bool firstBone);
