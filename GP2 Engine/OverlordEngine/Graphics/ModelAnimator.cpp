//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "ModelAnimator.h"
#include "../Diagnostics/Logger.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter(pMeshFilter),
	m_Transforms(vector<XMFLOAT4X4>()),
	m_IsPlaying(false), 
	m_Reversed(false),
	m_ClipSet(false),
	m_TickCount(0),
	m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;

	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		AnimationClip clip = m_pMeshFilter->m_AnimationClips[clipNumber];
		
		//	Call SetAnimation(AnimationClip clip)
		SetAnimation(clip);
	}
	else
	{
		// else
		//	Call Reset
		Reset(false);

		//	Log a warning with an appropriate message
		Logger::LogFormat(LogLevel::Warning, L"Couldn't find clip number %i in model animation!", clipNumber);

		//	return
		return;
	}
}

void ModelAnimator::SetAnimation(wstring clipName)
{
	// Set m_ClipSet to false
	m_ClipSet = false;

	// Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	bool found = false;
	AnimationClip clip = {};
	for (auto iter = m_pMeshFilter->m_AnimationClips.begin(); iter != m_pMeshFilter->m_AnimationClips.end(); ++iter)
	{
		if (iter->Name.compare(clipName) == 0)
		{
			found = true;
			clip = *iter;
			break;
		}
	}

	// If found,
	if (found)
	{
		// Call SetAnimation(Animation Clip) with the found clip
		SetAnimation(clip);
	}
	else
	{
		// Else
		// Call Reset
		Reset(false);

		// Log a warning with an appropriate message
		Logger::LogFormat(LogLevel::Warning, L"Couldn't find clip name %s in model animation!", clipName);
	}
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	
	//Set m_CurrentClip
	m_CurrentClip = clip;

	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause)
	{
		m_IsPlaying = false;
	}
	
	//Set m_TickCount to zero
	m_TickCount = 0.0f;

	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	m_Reversed = false;

	//If m_ClipSet is true
	if (m_ClipSet)
	{
		//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
		auto boneTransforms = m_CurrentClip.Keys[0].BoneTransforms;

		//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
		m_Transforms.assign(boneTransforms.begin(), boneTransforms.end());
	}
	else
	{
		//Else

		//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
		if (m_pMeshFilter->m_BoneCount > 0)
		{
			//	Create an IdentityMatrix 
			XMFLOAT4X4 identity;
			XMStoreFloat4x4(&identity, XMMatrixIdentity());

			m_Transforms.assign(m_pMeshFilter->m_BoneCount, identity);
		}
		else
		{
			Logger::LogWarning(L"Model animator's mesh filter has no bones!");
		}
	}
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	UNREFERENCED_PARAMETER(gameContext);

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		float passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;

		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);

		//2. 
		//IF m_Reversed is true
		if (m_Reversed)
		{
			//	Subtract passedTicks from m_TickCount
			m_TickCount -= passedTicks;

			//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
			if (m_TickCount < 0.0f)
			{
				m_TickCount += m_CurrentClip.Duration;
			}
		}
		else
		{
			//ELSE
			//	Add passedTicks to m_TickCount
			m_TickCount += passedTicks;

			//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
			if (m_TickCount > m_CurrentClip.Duration)
			{
				m_TickCount -= m_CurrentClip.Duration;
			}
		}

		//3.
		//Find the enclosing keys
		//Iterate all the keys of the clip and find the following keys:
		AnimationKey keyA, keyB;
		keyA.Tick = FLT_MIN;
		keyB.Tick = FLT_MAX;

		for (auto iter = m_CurrentClip.Keys.begin(); iter != m_CurrentClip.Keys.end(); ++iter)
		{
			AnimationKey key = *iter;

			//keyA > Closest Key with Tick before/smaller than m_TickCount
			if (key.Tick < m_TickCount && key.Tick > keyA.Tick)
			{
				keyA = key;
			}

			//keyB > Closest Key with Tick after/bigger than m_TickCount
			if (key.Tick > m_TickCount && key.Tick < keyB.Tick)
			{
				keyB = key;
			}
		}
		
		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		const float blendFactor = (m_TickCount - keyA.Tick) / (keyB.Tick - keyA.Tick);

		//Clear the m_Transforms vector
		m_Transforms.clear();

		//FOR every boneTransform in a key (So for every bone)
		for (size_t i = 0; i < keyA.BoneTransforms.size(); i++)
		{
			//	Retrieve the transform from keyA (transformA)
			auto transformA = keyA.BoneTransforms[i];

			// 	Retrieve the transform from keyB (transformB)
			auto transformB = keyB.BoneTransforms[i];

			// Decompose both transforms
			XMVECTOR scaleA;
			XMVECTOR rotateA;
			XMVECTOR translateA;
			XMMatrixDecompose(&scaleA, &rotateA, &translateA, XMLoadFloat4x4(&transformA));

			XMVECTOR scaleB;
			XMVECTOR rotateB;
			XMVECTOR translateB;
			XMMatrixDecompose(&scaleB, &rotateB, &translateB, XMLoadFloat4x4(&transformB));

			// Lerp between all the transformations (Position, Scale, Rotation)
			XMVECTOR finalScale = XMVectorLerp(scaleA, scaleB, blendFactor);
			XMVECTOR finalRotate = XMQuaternionSlerp(rotateA, rotateB, blendFactor);
			XMVECTOR finalTranslate = XMVectorLerp(translateA, translateB, blendFactor);

			// Compose a transformation matrix with the lerp-results
			XMMATRIX finalTransformM = XMMatrixScalingFromVector(finalScale) *
				XMMatrixRotationQuaternion(finalRotate) *
				XMMatrixTranslationFromVector(finalTranslate);

			XMFLOAT4X4 finalTransform;
			XMStoreFloat4x4(&finalTransform, finalTransformM);

			// Add the resulting matrix to the m_Transforms vector
			//m_Transforms.push_back(keyA.BoneTransforms[i]);
			m_Transforms.push_back(finalTransform);
		}
	}
}
