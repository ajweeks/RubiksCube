#pragma once

#include "BaseComponent.h"
#include "../Helpers/VertexHelper.h"

#include <vector>

class TextureData;

class TerrainComponent: public BaseComponent
{
public:
	TerrainComponent(const wstring& heightMapFile, const wstring& textureFile, unsigned int nrOfRows, unsigned int nrOfColumns, float width, float depth, float maxHeight);
	~TerrainComponent();

protected:

	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);
	virtual void Initialize(const GameContext& gameContext);

private:

	void ReadHeightMap();
	void CalculateVerticesAndIndices();
	void AddTriangle(unsigned int a, unsigned int b, unsigned int c);
	void AddQuad(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
	void BuildVertexBuffer(const GameContext& gameContext);
	void BuildIndexBuffer(const GameContext& gameContext);
	void CreatePxHeightField();

	// Resources
	const wstring m_HeightMapFile;
	const wstring m_TextureFile;

	// Terrain Properties
	unsigned int m_NrOfRows = 0;
	unsigned int m_NrOfColumns = 0;
	unsigned int m_NrOfVertices = 0;
	unsigned int m_NrOfTriangles = 0;
	float m_Width = 0.0f;
	float m_Depth = 0.0f;
	float m_MaxHeight = 0.0f;
	
	// Buffers
	vector<VertexPosNormTex> m_VecVertices;
	vector<DWORD> m_VecIndices;
	vector<unsigned short> m_VecHeightValues;
	vector<XMFLOAT3> m_VecFaceNormals;

	// Rendering
	TextureData* m_pTextureData;
	ID3D11Buffer*m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	unsigned int m_NumIndices = 0;

	// Physx
	GameObject* m_pTerrainObject = nullptr;
	PxHeightFieldSample* m_HeightFieldSamples = nullptr;

	ID3D11InputLayout* m_pInputLayout = nullptr;
	ID3DX11Effect* m_pEffect = nullptr;
	ID3DX11EffectTechnique* m_pTechnique = nullptr;
	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable = nullptr;
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable = nullptr;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseVariable = nullptr;


private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	TerrainComponent(const TerrainComponent& yRef);
	TerrainComponent& operator=(const TerrainComponent& yRef);

};

