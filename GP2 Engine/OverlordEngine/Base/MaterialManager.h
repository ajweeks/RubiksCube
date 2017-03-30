#pragma once
#include <map>

class Material;

class MaterialManager final
{

public:
	MaterialManager();
	~MaterialManager();

	void AddMaterial(Material *pMaterial, UINT materialId);
	void RemoveMaterial(UINT materialId);
	Material* GetMaterial(UINT materialId);

private:

	map<UINT, Material*> m_pMaterials;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	MaterialManager(const MaterialManager& t);
	MaterialManager& operator=(const MaterialManager& t);
};

