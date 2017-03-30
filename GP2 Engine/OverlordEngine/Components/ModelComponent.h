#pragma once
#include "BaseComponent.h"

class MeshFilter;
class ModelAnimator;
class Material;

class ModelComponent: public BaseComponent
{
public:
	ModelComponent(wstring assetFile);
	~ModelComponent();

	void SetMaterial(UINT materialId);
	ModelAnimator* GetAnimator() const { return m_pAnimator; }
	bool HasAnimator() const { return m_pAnimator != nullptr; }

protected:

	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
	void Initialize(const GameContext& gameContext) override;

private:

	void UpdateMaterial(const GameContext& gameContext);

	wstring m_AssetFile;
	MeshFilter *m_pMeshFilter;
	ModelAnimator* m_pAnimator;
	Material *m_pMaterial;
	UINT m_MaterialId;
	bool m_MaterialSet;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ModelComponent(const ModelComponent& yRef) = delete;
	ModelComponent& operator=(const ModelComponent& yRef) = delete;

};

