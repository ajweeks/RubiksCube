//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "TerrainComponent.h"

#include "..\Base\GeneralStructs.h"
#include "../Content/ContentManager.h"
#include "../Helpers/EffectHelper.h"
#include "TransformComponent.h"
#include "../Graphics/TextureData.h"
#include "../Physx/PhysxManager.h"
#include "../Physx/PhysxProxy.h"
#include "../Scenegraph/GameObject.h"
#include "../Scenegraph/GameScene.h"
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"


TerrainComponent::TerrainComponent(const wstring& heightMapFile, const wstring& textureFile, unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth, float maxHeight) :
	m_HeightMapFile(heightMapFile),
	m_TextureFile(textureFile),
	m_NrOfRows(nrOfRows),
	m_NrOfColumns(nrOfColumns),
	m_NrOfVertices(nrOfRows*nrOfColumns),
	m_NrOfTriangles(2 * (nrOfRows - 1) * (nrOfColumns - 1)),
	m_Width(width),
	m_Depth(depth),
	m_MaxHeight(maxHeight)
{
	
}

TerrainComponent::~TerrainComponent()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
	SafeRelease(m_pIndexBuffer);

	SafeDelete(m_HeightFieldSamples);
}

void TerrainComponent::Initialize(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);
	
	//Load Effect
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Resources/Effects/PosNormTex3D.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);

	//TODO: Error handling
	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorld")->AsMatrix();
	m_pDiffuseVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	UINT ilSize = 0;
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, ilSize))
	{
		Logger::LogError(L"TerrainComponent::Initialize >> BuildInputLayout failed!");
	}

	//Texture
	m_pTextureData = ContentManager::Load<TextureData>(m_TextureFile);

	//Load Height Map
	ReadHeightMap();

	//Create Vertices & Triangles
	CalculateVerticesAndIndices();

	//Build Vertexbuffer
	BuildVertexBuffer(gameContext);

	//Build Indexbuffer
	BuildIndexBuffer(gameContext);

	//Create PhysX Heightfield
	CreatePxHeightField();
}

void TerrainComponent::Update(const GameContext& gameContext)
{ 
	UNREFERENCED_PARAMETER(gameContext);
}

void TerrainComponent::Draw(const GameContext& gameContext)
{ 
	XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	XMMATRIX viewProj = XMLoadFloat4x4(&gameContext.pCamera->GetViewProjection());
	XMMATRIX wvp = XMMatrixMultiply(world, viewProj);
	m_pMatWorldVariable->SetMatrix(reinterpret_cast<float*>(&world));
	m_pMatWorldViewProjVariable->SetMatrix(reinterpret_cast<float*>(&wvp));
	m_pDiffuseVariable->SetResource(m_pTextureData->GetShaderResourceView());

	// Set vertex buffer
	UINT stride = sizeof(VertexPosNormTex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	// Set index buffer
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the input layout
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	// Set primitive topology
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render a cube
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p< techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}
}

//Exercise - Heightmap
void TerrainComponent::ReadHeightMap()
{
	m_VecHeightValues.resize(m_NrOfVertices, 0);

	std::ifstream inFileStream;
	inFileStream.open(m_HeightMapFile.c_str(), std::ios::binary);
	if (!inFileStream.is_open())
	{
		Logger::LogFormat(LogLevel::Error, L"Could not open heightmap file! %s", m_HeightMapFile.c_str());
		return;
	}

	inFileStream.read(reinterpret_cast<char*>(m_VecHeightValues.data()), static_cast<std::streamsize>(m_NrOfVertices * sizeof(unsigned short)));
	inFileStream.close();
}

//Exercise - Flat Grid
void TerrainComponent::CalculateVerticesAndIndices()
{
	//**VERTICES
	//Reserve spots in the buffer
	m_VecVertices.reserve(m_NrOfVertices);
	m_VecFaceNormals.reserve(m_NrOfVertices);
	m_VecHeightValues.reserve(m_NrOfVertices);

	//Calculate the Initial Position (Terrain centered on the origin)
	//Reset the cellXPos Position for each Column
	const XMFLOAT3 initialPos = XMFLOAT3(-m_Width / 2.0f, 0.0f, m_Depth / 2.0f);
	XMFLOAT3 currentPos = initialPos;

	const float dZ = m_Depth / (float)m_NrOfRows;
	const float dX = m_Width / (float)m_NrOfColumns;

	for (size_t x = 0; x < m_NrOfColumns; x++)
	{
		for (size_t z = 0; z < m_NrOfRows; z++)
		{
			const size_t currentIndex = x * m_NrOfRows + z;

			currentPos.y = (m_VecHeightValues[currentIndex] / (float)std::numeric_limits<unsigned short>::max()) * m_MaxHeight;
			VertexPosNormTex vertex = {};
			vertex.Position = currentPos;
			vertex.TexCoord = XMFLOAT2((float)z / (float)(m_NrOfRows - 1), (float)x / (float)(m_NrOfColumns - 1));

			m_VecVertices.push_back(vertex);

			currentPos.z -= dZ;
		}
		currentPos.x += dX;
		currentPos.z = initialPos.z;
	}

	const unsigned int numIndices = m_NrOfTriangles * 3;
	for (size_t x = 0; x < m_NrOfColumns - 1; x++)
	{
		for (size_t z = 0; z < m_NrOfRows - 1; z++)
		{
			const unsigned int a = x * m_NrOfColumns + z;
			const unsigned int b = a + 1;
			const unsigned int c = a + m_NrOfColumns;
			const unsigned int d = c + 1;

			AddQuad(a, b, c, d);
		}
	}

	//Exercise - Normals
	for (size_t i = 0; i < m_VecIndices.size(); i += 6)
	{
		XMVECTOR vertex0 = XMLoadFloat3(&m_VecVertices[m_VecIndices[i]].Position);
		XMVECTOR vertex1 = XMLoadFloat3(&m_VecVertices[m_VecIndices[i + 1]].Position);
		XMVECTOR vertex2 = XMLoadFloat3(&m_VecVertices[m_VecIndices[i + 2]].Position);
		XMVECTOR vertex3 = XMLoadFloat3(&m_VecVertices[m_VecIndices[i + 3]].Position);
		XMVECTOR vertex4 = XMLoadFloat3(&m_VecVertices[m_VecIndices[i + 4]].Position);
		XMVECTOR vertex5 = XMLoadFloat3(&m_VecVertices[m_VecIndices[i + 5]].Position);

		XMVECTOR v1 = vertex0 - vertex2;
		XMVECTOR v2 = vertex0 - vertex1;

		XMVECTOR normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);

		XMFLOAT3 normalFloat3;
		XMStoreFloat3(&normalFloat3, normal);
		m_VecFaceNormals.push_back(normalFloat3);

		v1 = vertex5 - vertex4;
		v2 = vertex5 - vertex3;

		normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);

		XMStoreFloat3(&normalFloat3, normal);
		m_VecFaceNormals.push_back(normalFloat3);
	}

	//calculate average by normalizing
	vector<XMFLOAT3> averagedNormals;
	const size_t numFacesPerRow = (m_NrOfRows - 1) * 2;
	int index[6];
	for (size_t x = 0; x < m_NrOfColumns; x++)
	{
		for (size_t z = 0; z < m_NrOfRows; z++)
		{
			const size_t currentIndex = x * m_NrOfRows + z;
			const size_t centerFaceIndex = numFacesPerRow * x + z * 2;
			index[0] = centerFaceIndex - 1;
			index[1] = centerFaceIndex;
			index[2] = centerFaceIndex + 1;
			index[3] = centerFaceIndex - numFacesPerRow - 2;
			index[4] = centerFaceIndex - numFacesPerRow - 1;
			index[5] = centerFaceIndex - numFacesPerRow;
	
			if (z == 0)
			{
				index[0] = -1;
				index[3] = -1;
				index[4] = -1;
			}
	
			if (z == m_NrOfRows - 1)
			{
				index[1] = -1;
				index[2] = -1;
				index[5] = -1;
			}
	
			XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0, 0.0f);
			for (size_t i = 0; i < 6; i++)
			{
				if (index[i] >= 0 && index[i] < (int)m_VecFaceNormals.size())
				{
					normalSum += XMLoadFloat3(&m_VecFaceNormals[index[i]]);
				}
			}
	
			XMVECTOR averagedNormal = XMVector3Normalize(normalSum);
			XMStoreFloat3(&m_VecVertices[currentIndex].Normal, averagedNormal);
		}
	}
}

//Exercise - Flat Grid
void TerrainComponent::AddTriangle(unsigned int a, unsigned int b, unsigned int c)
{
	m_VecIndices.push_back(a);
	m_VecIndices.push_back(b);
	m_VecIndices.push_back(c);
}

//Exercise - Flat Grid
void TerrainComponent::AddQuad(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
	AddTriangle(a, d, c);
	AddTriangle(a, b, d);
}

void TerrainComponent::BuildVertexBuffer(const GameContext& gameContext)
{
	//Vertexbuffer
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(VertexPosNormTex) * m_NrOfVertices;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecVertices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Vertexbuffer");
}

void TerrainComponent::BuildIndexBuffer(const GameContext& gameContext)
{
	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_VecIndices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_VecIndices.data();
	HRESULT hr = gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	Logger::LogHResult(hr, L"Failed to Create Indexbuffer");
	m_NumIndices = m_VecIndices.size();
}

//Exercise - PhysX
void TerrainComponent::CreatePxHeightField()
{
	PxPhysics* physx = PhysxManager::GetInstance()->GetPhysics();

	m_HeightFieldSamples = new PxHeightFieldSample[sizeof(PxHeightFieldSample) * (m_NrOfColumns * m_NrOfRows)];
	
	const unsigned short maxUshort = std::numeric_limits<unsigned short>::max();
	const unsigned short halfMaxUshort = maxUshort / 2;
	for (size_t x = 0; x < m_NrOfColumns; x++)
	{
		for (size_t z = 0; z < m_NrOfRows; z++)
		{
			const size_t currentIndex = x * m_NrOfRows + z;
			const size_t alternateIndex = x * m_NrOfRows + (m_NrOfRows - z);

			const PxI16 sampledHeight = (PxI16)((m_VecHeightValues[currentIndex]/2));
			m_HeightFieldSamples[alternateIndex].height = sampledHeight;
		}
	}

	PxHeightFieldDesc heightfieldDesc = {};
	heightfieldDesc.format = PxHeightFieldFormat::eS16_TM;
	heightfieldDesc.nbColumns = m_NrOfColumns;
	heightfieldDesc.nbRows = m_NrOfRows;
	heightfieldDesc.samples.data = m_HeightFieldSamples;
	heightfieldDesc.samples.stride = sizeof(PxHeightFieldSample);
	
	PxHeightField* heightField = physx->createHeightField(heightfieldDesc);
	std::shared_ptr<PxGeometry> heightFieldGeometry(new PxHeightFieldGeometry(heightField, PxMeshGeometryFlags(), 
		0.00092f, 0.78f, 0.78f));
	
	PxMaterial* defaultMaterial = physx->createMaterial(0.5f, 0.5f, 0.1f);
	ColliderComponent* colliderComponent = new ColliderComponent(heightFieldGeometry, *defaultMaterial, 
		PxTransform(PxVec3(-m_Width / 2.0f, 0.0, -m_Depth / 2.0f)));
	RigidBodyComponent* rbComponent = new RigidBodyComponent(true);

	m_pTerrainObject = new GameObject();
	m_pTerrainObject->AddComponent(rbComponent);
	m_pTerrainObject->AddComponent(colliderComponent);

	m_pGameObject->AddChild(m_pTerrainObject);
	
	//After implementing the physX representation - Fix the lagginess from the PhysX Debugger!!!
}
