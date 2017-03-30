#pragma once
#include "../Helpers/EffectHelper.h"

class MeshFilter;
class ModelComponent;
struct GameContext;

class Material
{
public:
	Material(const std::wstring& effectFile, const std::wstring& technique = L"");
	virtual ~Material(void);

	void Initialize(const GameContext& gameContext);
	void SetEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);
	ID3DX11EffectTechnique* GetTechnique() const { return m_pTechnique; }
	ID3D11InputLayout* GetInputLayout(){ return m_pInputLayout; }


protected:

	virtual void LoadEffectVariables() = 0;
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) = 0;

	ID3DX11Effect *m_pEffect;
	ID3DX11EffectTechnique *m_pTechnique;

private:

	friend class MeshFilter;

	bool LoadEffect(const GameContext& gameContext, wstring effectFile);

	ID3DX11EffectMatrixVariable *m_pWorldMatrixVariable;
	ID3DX11EffectMatrixVariable *m_pViewMatrixVariable;
	ID3DX11EffectMatrixVariable *m_pViewInverseMatrixVariable;
	ID3DX11EffectMatrixVariable *m_pWvpMatrixVariable;

	ID3D11InputLayout *m_pInputLayout;
	vector<ILDescription> m_pInputLayoutDescriptions;
	UINT m_pInputLayoutSize;

	wstring m_effectFile;
	wstring m_TechniqueName;
	bool m_IsInitialized;
	UINT m_InputLayoutID;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	Material(const Material& yRef);
	Material& operator=(const Material& yRef);
};

