#pragma once
#include <vector>
#include "..\Helpers\EffectHelper.h"

class MeshFilterLoader;
class Material;
struct GameContext;
class ModelComponent;
class DerezModelComponent;

struct VertexBufferData
{
	VertexBufferData():
		pDataStart(nullptr),
		pVertexBuffer(nullptr),
		BufferSize(0),
		VertexStride(0),
		VertexCount(0),
		IndexCount(0),
		InputLayoutID(0){}

	void* pDataStart;
	ID3D11Buffer* pVertexBuffer;
	UINT BufferSize;
	UINT VertexStride;
	UINT VertexCount;
	UINT IndexCount;
	UINT InputLayoutID;

	void VertexBufferData::Destroy()
	{
		free(pDataStart);
		SafeRelease(pVertexBuffer);
	}
};

struct AnimationKey
{
	float Tick;
	vector<XMFLOAT4X4> BoneTransforms;
};

struct AnimationClip
{
	AnimationClip(){};

	wstring Name;
	float Duration;
	float TicksPerSecond;
	vector<AnimationKey> Keys;
};

class MeshFilter final
{
public:
	MeshFilter();
	~MeshFilter();

private:

	friend class MeshFilterLoader;
	friend class ModelComponent;
	friend class ModelAnimator;
	friend class DerezModelComponent;

	int GetVertexBufferId(UINT inputLayoutId);
	void BuildVertexBuffer(const GameContext& gameContext, Material* pMaterial);
	void BuildIndexBuffer(const GameContext& gameContext);
	bool HasElement(ILSemantic element) { return (m_HasElement&element) > 0 ? true : false; }
	const VertexBufferData& GetVertexBufferData(const GameContext& gameContext, Material* pMaterial);

	//VERTEX DATA
	UINT m_VertexCount, m_IndexCount;
	UINT m_HasElement;
	vector<XMFLOAT3> m_Positions;
	vector<XMFLOAT3> m_Normals;
	vector<XMFLOAT3> m_Tangents;
	vector<XMFLOAT3> m_Binormals;
	vector<XMFLOAT2> m_TexCoords;
	vector<XMFLOAT4> m_Colors;
	vector<XMFLOAT4> m_BlendIndices;
	vector<XMFLOAT4> m_BlendWeights;
	vector<AnimationClip> m_AnimationClips;
	bool m_HasAnimations;
	USHORT m_BoneCount;

	//INDEX DATA
	vector<DWORD> m_Indices;

	vector<VertexBufferData> m_VertexBuffers;
	ID3D11Buffer* m_pIndexBuffer;

	wstring m_MeshName;
	static XMFLOAT4 m_DefaultColor;
	static XMFLOAT4 m_DefaultFloat4;
	static XMFLOAT3 m_DefaultFloat3;
	static XMFLOAT2 m_DefaultFloat2;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MeshFilter(const MeshFilter &obj);
	MeshFilter& operator=(const MeshFilter& obj);
};

