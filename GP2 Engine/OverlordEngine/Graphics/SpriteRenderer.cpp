//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "SpriteRenderer.h"
#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "TextureData.h"
#include <algorithm>
#include "../Base/OverlordGame.h"

SpriteRenderer::SpriteRenderer() :
	m_Sprites(vector<SpriteVertex>()),
	m_Textures(vector<TextureData*>()),
	m_pEffect(nullptr),
	m_pTechnique(nullptr),
	m_pInputLayout(nullptr),
	m_pVertexBuffer(nullptr),
	m_BufferSize(50),
	m_InputLayoutSize(0),
	m_pTransfromMatrixV(nullptr),
	m_Transform(XMFLOAT4X4()),
	m_pTextureSizeV(nullptr),
	m_pTextureSRV(nullptr)
{
}

SpriteRenderer::~SpriteRenderer()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);

	m_Sprites.clear();
	m_Textures.clear();
}

void SpriteRenderer::InitRenderer(ID3D11Device* pDevice)
{
	//Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/SpriteRenderer.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	m_pTransfromMatrixV = m_pEffect->GetVariableByName("gTransform")->AsMatrix();
	if (!m_pTransfromMatrixV->IsValid())
	{
		Logger::LogError(L"SpriteRenderer::Initialize() > Shader variable \'gTransform\' not valid!");
		return;
	}

	m_pTextureSizeV = m_pEffect->GetVariableByName("gTextureSize")->AsVector();
	if (!m_pTextureSizeV->IsValid())
	{
		Logger::LogError(L"SpriteRenderer::Initialize() > Shader variable \'gTextureSize\' not valid!");
		return;
	}

	m_pTextureSRV = m_pEffect->GetVariableByName("gSpriteTexture")->AsShaderResource();
	if (!m_pTextureSRV->IsValid())
	{
		Logger::LogError(L"SpriteRenderer::Initialize() > Shader variable \'gSpriteTexture\' not valid!");
		return;
	}

	EffectHelper::BuildInputLayout(pDevice, m_pTechnique, &m_pInputLayout, m_InputLayoutSize);
	
	//Transform Matrix
	auto settings = OverlordGame::GetGameSettings();
	float scaleX = (settings.Window.Width>0) ? 2.0f / settings.Window.Width : 0;
	float scaleY = (settings.Window.Height>0) ? 2.0f / settings.Window.Height : 0;

	m_Transform._11 = scaleX; m_Transform._12 = 0; m_Transform._13 = 0; m_Transform._14 = 0;
	m_Transform._21 = 0; m_Transform._22 = -scaleY; m_Transform._23 = 0; m_Transform._24 = 0;
	m_Transform._31 = 0; m_Transform._32 = 0; m_Transform._33 = 1; m_Transform._34 = 0;
	m_Transform._41 = -1; m_Transform._42 = 1; m_Transform._43 = 0; m_Transform._44 = 1;
}

void SpriteRenderer::UpdateBuffer(const GameContext& gameContext)
{
	// if the vertex buffer does not exists, or the number of sprites is bigger then the buffer size
	//		release the buffer
	//		update the buffer size (if needed)
	//		Create a new buffer. Make sure the Usage flag is set to Dynamic, you bind to the vertex buffer
	//		and you set the cpu access flags to access_write
	if (!m_pVertexBuffer || m_Sprites.size() > m_BufferSize)
	{
		SafeRelease(m_pVertexBuffer);

		m_BufferSize = m_Sprites.size();

		D3D11_BUFFER_DESC vertexBufferDesc = {};
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = sizeof(SpriteVertex) * m_BufferSize;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = m_Sprites.data();
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		// Finally create the buffer. You can use the device in the game context. Be sure to log the HResult!
		HRESULT hr = gameContext.pDevice->CreateBuffer(&vertexBufferDesc, &initData, &m_pVertexBuffer);
		if (Logger::LogHResult(hr, L"SpriteRenderer::UpdateBuffer")) return;

		// Finally fill the  buffer. You will need to create a D3D11_MAPPED_SUBRESOURCE
		// Next you will need to use the device context to map the vertex buffer to the mapped resource
		// use memcpu to copy all our sprites to the mapped resource
		// unmap the vertex buffer
		D3D11_MAPPED_SUBRESOURCE mappedSubresource = {};
		gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE, 0, &mappedSubresource);
	}
}

void SpriteRenderer::Draw(const GameContext& gameContext)
{
	if (m_Sprites.size() <= 0)
		return;

	UpdateBuffer(gameContext);

	//Set Render Pipeline
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(SpriteVertex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	UINT batchSize = 1;
	UINT batchOffset = 0;
	UINT spriteCount = m_Sprites.size();
	for (UINT i = 0; i < spriteCount; ++i)
	{
		if (i < (spriteCount - 1) && m_Sprites[i].TextureId == m_Sprites[i + 1].TextureId)
		{
			++batchSize;
			continue;
		}

		//Set Texture
		auto texData = m_Textures[m_Sprites[i].TextureId];
		m_pTextureSRV->SetResource(texData->GetShaderResourceView());

		//Set Texture Size
		auto texSize = texData->GetDimension();
		m_pTextureSizeV->SetFloatVector(reinterpret_cast<float*>(&texSize));

		//Set Transform
		m_pTransfromMatrixV->SetMatrix(reinterpret_cast<float*>(&m_Transform));

		D3DX11_TECHNIQUE_DESC techDesc;
		m_pTechnique->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
			gameContext.pDeviceContext->Draw(batchSize, batchOffset);
		}

		batchOffset += batchSize;
		batchSize = 1;
	}

	m_Sprites.clear();
	m_Textures.clear();
}

void SpriteRenderer::Draw(TextureData* pTexture, XMFLOAT2 position, XMFLOAT4 color, XMFLOAT2 pivot, XMFLOAT2 scale, float rotation, float depth)
{
	SpriteVertex vertex;

	auto it = find(m_Textures.begin(), m_Textures.end(), pTexture);
	
	if (it == m_Textures.end())
	{
		m_Textures.push_back(pTexture);
		vertex.TextureId = m_Textures.size() - 1;
	}
	else
	{
		vertex.TextureId = it - m_Textures.begin();
	}

	vertex.TransformData = XMFLOAT4(position.x, position.y, depth, rotation);
	vertex.TransformData2 = XMFLOAT4(pivot.x, pivot.y, scale.x, scale.y);
	vertex.Color = color;

	m_Sprites.push_back(vertex);
}
