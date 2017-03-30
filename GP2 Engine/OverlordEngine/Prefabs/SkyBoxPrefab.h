#pragma once
#include "..\Scenegraph\GameObject.h"

class CubeMapMaterial;

class SkyBoxPrefab : public GameObject
{
public:
	SkyBoxPrefab(const wstring& textureFilepath);
	~SkyBoxPrefab();

	void SetTextureFilepath(const wstring& textureFilepath);

protected:
	virtual void Initialize(const GameContext& gameContext);

private:
	wstring m_TextureFilepath;
	CubeMapMaterial* m_CubeMapMaterial;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	SkyBoxPrefab(const SkyBoxPrefab& yRef);
	SkyBoxPrefab& operator=(const SkyBoxPrefab& yRef);
};
