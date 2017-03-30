#pragma once

struct VertexPosCol;
struct GameContext;

class DebugRenderer
{
public:

	static void InitRenderer(ID3D11Device *pDevice, UINT bufferSize = 100);
	static void ToggleDebugRenderer();
	static void EnableDebugRenderer(bool enabled);

	static void Release();

	//RENDERING
	static void Draw(const GameContext& gameContext);
	static void DrawPhysX(PxScene* pScene);

private:
	DebugRenderer();
	~DebugRenderer();

	//RENDERING
	static void CreateVertexBuffer(ID3D11Device *pDevice);
	static void CreateFixedLineList();
	static XMFLOAT4 ConvertPxColor(PxU32 color);
	static ID3DX11Effect* m_pEffect;
	static ID3DX11EffectTechnique* m_pTechnique;
	static UINT m_BufferSize, m_FixedBufferSize;
	static ID3D11InputLayout *m_pInputLayout;
	static ID3D11Buffer *m_pVertexBuffer;
	static ID3DX11EffectMatrixVariable *m_pWvpVariable;
	static vector<VertexPosCol> m_LineList;
	static vector<VertexPosCol> m_FixedLineList;
	static bool m_RendererEnabled;

	static void DrawLine(const XMFLOAT3& start, const XMFLOAT3& end, const XMFLOAT4& color);
	static void DrawLine(const XMFLOAT3& start, const XMFLOAT4& colorStart, const XMFLOAT3& end, const XMFLOAT4& colorEnd);

	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	DebugRenderer(const DebugRenderer& t);
	DebugRenderer& operator=(const DebugRenderer& t);
};

