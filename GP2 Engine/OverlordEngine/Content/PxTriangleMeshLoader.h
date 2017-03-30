#pragma once
#include "ContentLoader.h"

class PxTriangleMeshLoader : public ContentLoader<PxTriangleMesh>
{
public:
	PxTriangleMeshLoader(void);
	~PxTriangleMeshLoader(void);

protected:

	virtual PxTriangleMesh* LoadContent(const wstring& assetFile);
	virtual void Destroy(PxTriangleMesh* objToDestroy);

private:
// -------------------------
// Disabling default copy constructor and default 
// assignment operator.
// -------------------------
PxTriangleMeshLoader(const PxTriangleMeshLoader& yRef);									
PxTriangleMeshLoader& operator=(const PxTriangleMeshLoader& yRef);
};

