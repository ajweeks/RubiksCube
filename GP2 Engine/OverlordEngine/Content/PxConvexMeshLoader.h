#pragma once
#include "ContentLoader.h"

class PxConvexMeshLoader : public ContentLoader<PxConvexMesh>
{
public:
	PxConvexMeshLoader(void);
	~PxConvexMeshLoader(void);

protected:

	virtual PxConvexMesh* LoadContent(const wstring& assetFile);
	virtual void Destroy(PxConvexMesh* objToDestroy);

private:
// -------------------------
// Disabling default copy constructor and default 
// assignment operator.
// -------------------------
PxConvexMeshLoader(const PxConvexMeshLoader& yRef);									
PxConvexMeshLoader& operator=(const PxConvexMeshLoader& yRef);
};

