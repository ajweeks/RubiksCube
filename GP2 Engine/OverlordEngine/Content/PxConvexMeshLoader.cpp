#include "stdafx.h"
#include "PxConvexMeshLoader.h"
#include "..\Physx\PhysxManager.h"

PxConvexMeshLoader::PxConvexMeshLoader(void)
{
}


PxConvexMeshLoader::~PxConvexMeshLoader(void)
{
}

PxConvexMesh* PxConvexMeshLoader::LoadContent(const wstring& assetFile)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	string buffer = string(assetFile.begin(), assetFile.end());
	auto inputStream  = PxDefaultFileInputData(buffer.c_str());
	auto convexMesh = physX->createConvexMesh(inputStream);

	return convexMesh;
}

void PxConvexMeshLoader::Destroy(PxConvexMesh* objToDestroy)
{
	UNREFERENCED_PARAMETER(objToDestroy);
	//PhysX handles its own resources
}
