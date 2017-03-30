#pragma once
#include "MeshFilter.h"

class ModelAnimator
{
public:
	ModelAnimator(MeshFilter* pMeshFilter);
	~ModelAnimator();

	void SetAnimation(wstring clipName);
	void SetAnimation(UINT clipNumber);
	void SetAnimation(AnimationClip clip);
	void Update(const GameContext& gameContext);
	void Reset(bool pause = true);
	void Play() { m_IsPlaying = true; }
	void Pause() { m_IsPlaying = false; }
	void SetPlayReversed(bool reverse) { m_Reversed = reverse; }
	void SetAnimationSpeed(float speedPercentage) { m_AnimationSpeed = speedPercentage; }

	bool IsPlaying() const { return m_IsPlaying; }
	bool IsReversed() const { return m_Reversed; }
	float GetAnimationSpeed() const { return m_AnimationSpeed; }
	UINT GetClipCount() const { return m_pMeshFilter->m_AnimationClips.size(); }
	wstring GetClipName() const { return m_ClipSet?m_CurrentClip.Name:L""; }
	vector<XMFLOAT4X4> GetBoneTransforms() const { return m_Transforms; }

private:

	AnimationClip m_CurrentClip;
	MeshFilter* m_pMeshFilter;
	vector<XMFLOAT4X4> m_Transforms;
	bool m_IsPlaying, m_Reversed, m_ClipSet;
	float m_TickCount, m_AnimationSpeed;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	ModelAnimator(const ModelAnimator& yRef);
	ModelAnimator& operator=(const ModelAnimator& yRef);
};

