//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.3.133
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
#include "stdafx.h"
#include "TextRenderer.h"

#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "../Base/OverlordGame.h"
#include "SpriteFont.h"
#include "TextureData.h"


TextRenderer::TextRenderer() :
m_pEffect(nullptr),
m_pTechnique(nullptr),
m_pTransfromMatrixV(nullptr),
m_pTextureSizeV(nullptr),
m_pTextureSRV(nullptr),
m_pInputLayout(nullptr),
m_pVertexBuffer(nullptr),
m_BufferSize(500),
m_InputLayoutSize(0),
m_Transform(XMFLOAT4X4()),
m_NumCharacters(0),
m_SpriteFonts(vector<SpriteFont*>())
{
}


TextRenderer::~TextRenderer()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void TextRenderer::InitRenderer(ID3D11Device* pDevice)
{
	//Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/TextRenderer.fx");
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

void TextRenderer::DrawText(SpriteFont* pFont, const wstring& text, XMFLOAT2 position, XMFLOAT4 color)
{
	m_NumCharacters += wcslen(text.c_str());
	pFont->m_TextCache.push_back(TextCache(text, position, color));
	if (!pFont->m_IsAddedToRenderer)
	{
		m_SpriteFonts.push_back(pFont);
		pFont->m_IsAddedToRenderer = true;
	}
}

void TextRenderer::Draw(const GameContext& gameContext)
{
	if (m_SpriteFonts.size() <= 0)
		return;

	UpdateBuffer(gameContext);

	//Set Render Pipeline
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	UINT stride = sizeof(TextVertex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	for each(SpriteFont* pFont in m_SpriteFonts)
	{
		//Set Texture
		m_pTextureSRV->SetResource(pFont->m_pTexture->GetShaderResourceView());

		//Set Texture Size
		auto texSize = pFont->m_pTexture->GetDimension();
		m_pTextureSizeV->SetFloatVector(reinterpret_cast<float*>(&texSize));

		//Set Transform
		m_pTransfromMatrixV->SetMatrix(reinterpret_cast<float*>(&m_Transform));

		D3DX11_TECHNIQUE_DESC techDesc;
		m_pTechnique->GetDesc(&techDesc);
		for (UINT i = 0; i < techDesc.Passes; ++i)
		{
			m_pTechnique->GetPassByIndex(i)->Apply(0, gameContext.pDeviceContext);
			gameContext.pDeviceContext->Draw(pFont->m_BufferSize, pFont->m_BufferStart);
		}

		pFont->m_IsAddedToRenderer = false;
	}

	m_SpriteFonts.clear();
}

void TextRenderer::UpdateBuffer(const GameContext& gameContext)
{
	if (!m_pVertexBuffer || m_NumCharacters > m_BufferSize)
	{
		//Release the buffer if it exists
		SafeRelease(m_pVertexBuffer);

		//Set buffersize if needed
		if (m_NumCharacters > m_BufferSize)
			m_BufferSize = m_NumCharacters;

		//Create a new buffer
		D3D11_BUFFER_DESC buffDesc;
		buffDesc.Usage = D3D11_USAGE_DYNAMIC;
		buffDesc.ByteWidth = sizeof(TextVertex) * m_BufferSize;
		buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffDesc.MiscFlags = 0;

		auto hr = gameContext.pDevice->CreateBuffer(&buffDesc, nullptr, &m_pVertexBuffer);
		if (Logger::LogHResult(hr, L"TextRenderer::UpdateBuffer"))
			return;
	}

	//Fill Buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	int bufferPosition = 0;
	for each (SpriteFont* pFont in m_SpriteFonts)
	{
		pFont->m_BufferStart = bufferPosition;
		for each(TextCache cache in pFont->m_TextCache)
		{
			CreateTextVertices(pFont, cache, (TextVertex*) mappedResource.pData, bufferPosition);
		}

		pFont->m_BufferSize = bufferPosition - pFont->m_BufferStart;
		pFont->m_TextCache.clear(); //Clear the cache
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);

	m_NumCharacters = 0;
}

void TextRenderer::CreateTextVertices(SpriteFont* pFont, const TextCache& info, TextVertex* pBuffer, int& bufferPosition)
{
	int totalAdvanceX = 0;
	for (wchar_t charId : info.Text)
	{
		if (SpriteFont::IsCharValid(charId) && pFont->GetMetric(charId).IsValid)
		{
			auto metric = pFont->GetMetric(charId);

			if (charId == ' ')
			{
				totalAdvanceX += metric.AdvanceX;
				continue;
			}

			TextVertex vertexText;
			vertexText.Position.x = info.Position.x + totalAdvanceX + metric.OffsetX;
			vertexText.Position.y = info.Position.y + metric.OffsetY;
			vertexText.Position.z = 0.9f;
			vertexText.Color = info.Color;
			vertexText.TexCoord = metric.TexCoord;
			vertexText.CharacterDimension = XMFLOAT2(metric.Width, metric.Height);
			vertexText.ChannelId = metric.Channel;

			pBuffer[bufferPosition] = vertexText;

			++bufferPosition;
			totalAdvanceX += metric.AdvanceX;

		}
		else
		{
			Logger::LogFormat(LogLevel::Warning, L"TextRenderer::CreateTextVertices > Char not supported for current font.\nCHARACTER: %c (%i)\nFONT: %s", charId, (int) charId, pFont->m_FontName.c_str());
		}
	}
}
