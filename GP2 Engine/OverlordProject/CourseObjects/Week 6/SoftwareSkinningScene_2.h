#pragma once

#include "Scenegraph/GameScene.h"
#include "Helpers\SkinnedVertex.h"
#include "Components\MeshDrawComponent.h"

class BoneObject;

class SoftwareSkinningScene_2 : public GameScene
{
public:
	SoftwareSkinningScene_2();
	virtual ~SoftwareSkinningScene_2();

protected:
	virtual void Initialize(const GameContext& gameContext);
	virtual void Update(const GameContext& gameContext);
	virtual void Draw(const GameContext& gameContext);

private:
	void CreateMesh(float length); 

	BoneObject* m_pBone0, *m_pBone1;
	float m_BoneRotation;
	int m_RotationSign;
	MeshDrawComponent* m_pMeshDrawComponent;
	vector<SkinnedVertex> m_SkinnedVertices;


	SoftwareSkinningScene_2(const SoftwareSkinningScene_2 &obj);
	SoftwareSkinningScene_2& operator=(const SoftwareSkinningScene_2& obj);
};
