#pragma once

#include "..\Scenegraph\GameObject.h"

class BoneObject : public GameObject
{
public:
	BoneObject(int boneID, int materialID, float length = 5.0f);
	~BoneObject();

	void AddBone(BoneObject* pBone);

	XMFLOAT4X4 GetBindPose() const;
	void CalculateBindPose();

protected:
	virtual void Initialize(const GameContext& gameContext) override;

private:
	int m_BoneID;
	int m_MaterialID;
	float m_Length;
	XMFLOAT4X4 m_BindPose;

	BoneObject(const BoneObject&) = delete;
	BoneObject& operator=(const BoneObject&) = delete;
};	