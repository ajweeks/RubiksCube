#pragma once

#include "Graphics\Material.h"
#include "Base/GeneralStructs.h"

class ColorMaterial: public Material
{
public:
	ColorMaterial(bool enableTransparency = false);
	~ColorMaterial();

protected:
	virtual void LoadEffectVariables();
	virtual void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent);

private:

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ColorMaterial(const ColorMaterial &obj);
	ColorMaterial& operator=(const ColorMaterial& obj);
};

