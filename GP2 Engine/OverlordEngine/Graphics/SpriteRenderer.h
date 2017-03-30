#pragma once
#include "../Helpers/Singleton.h"

class TextureData;
class OverlordGame;
class GameScene;

class SpriteRenderer: public Singleton<SpriteRenderer>
{
public:

	void Draw(TextureData* pTexture, XMFLOAT2 position, XMFLOAT4 color = (XMFLOAT4)Colors::White, XMFLOAT2 pivot = XMFLOAT2(0,0), XMFLOAT2 scale = XMFLOAT2(1,1), float rotation = 0.f, float depth = 0.f);

	void InitRenderer(ID3D11Device* pDevice);
	void Draw(const GameContext& gameContext);

private:
	void UpdateBuffer(const GameContext& gameContext);

	struct SpriteVertex
	{
		UINT TextureId;
		XMFLOAT4 TransformData;
		XMFLOAT4 TransformData2;
		XMFLOAT4 Color;
	};

	friend class Singleton<SpriteRenderer>;
	friend class OverlordGame;
	friend class GameScene;

	SpriteRenderer();
	~SpriteRenderer();
	vector<SpriteVertex> m_Sprites;
	vector<TextureData*> m_Textures;
	UINT m_BufferSize, m_InputLayoutSize;
	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pVertexBuffer;

	ID3DX11EffectMatrixVariable* m_pTransfromMatrixV;
	XMFLOAT4X4 m_Transform;
	ID3DX11EffectVectorVariable* m_pTextureSizeV;
	ID3DX11EffectShaderResourceVariable* m_pTextureSRV;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SpriteRenderer(const SpriteRenderer& t);
	SpriteRenderer& operator=(const SpriteRenderer& t);
};

