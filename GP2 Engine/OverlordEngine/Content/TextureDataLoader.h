#pragma once
#include "ContentLoader.h"
#include "..\Graphics\TextureData.h"

class TextureDataLoader : public ContentLoader<TextureData>
{
public:
	TextureDataLoader(void);
	~TextureDataLoader(void);

protected:

	virtual TextureData* LoadContent(const wstring& assetFile);
	virtual void Destroy(TextureData* objToDestroy);

private:
// -------------------------
// Disabling default copy constructor and default 
// assignment operator.
// -------------------------
TextureDataLoader(const TextureDataLoader& yRef);									
TextureDataLoader& operator=(const TextureDataLoader& yRef);
};

