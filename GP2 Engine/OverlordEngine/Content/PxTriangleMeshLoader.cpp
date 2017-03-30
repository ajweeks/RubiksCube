#include "stdafx.h"
#include "PxTriangleMeshLoader.h"
#include "..\Physx\PhysxManager.h"


PxTriangleMeshLoader::PxTriangleMeshLoader(void)
{
}


PxTriangleMeshLoader::~PxTriangleMeshLoader(void)
{
}

PxTriangleMesh* PxTriangleMeshLoader::LoadContent(const wstring& assetFile)
{
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	string buffer = string(assetFile.begin(), assetFile.end());
	auto inputStream  = PxDefaultFileInputData(buffer.c_str());
	auto triangleMesh = physX->createTriangleMesh(inputStream);

	return triangleMesh;
}

void PxTriangleMeshLoader::Destroy(PxTriangleMesh* objToDestroy)
{
	UNREFERENCED_PARAMETER(objToDestroy);
	//PhysX handles its own resources
}
