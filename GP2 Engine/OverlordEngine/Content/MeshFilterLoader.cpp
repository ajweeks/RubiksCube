//------------------------------------------------------------------------------------------------------
//   _____     _______ ____  _     ___  ____  ____    _____ _   _  ____ ___ _   _ _____   ______  ___ _ 
//  / _ \ \   / / ____|  _ \| |   / _ \|  _ \|  _ \  | ____| \ | |/ ___|_ _| \ | | ____| |  _ \ \/ / / |
// | | | \ \ / /|  _| | |_) | |  | | | | |_) | | | | |  _| |  \| | |  _ | ||  \| |  _|   | | | \  /| | |
// | |_| |\ V / | |___|  _ <| |__| |_| |  _ <| |_| | | |___| |\  | |_| || || |\  | |___  | |_| /  \| | |
//  \___/  \_/  |_____|_| \_\_____\___/|_| \_\____/  |_____|_| \_|\____|___|_| \_|_____| |____/_/\_\_|_|
//
// Overlord Engine v1.115
// Copyright Overlord Thomas Goussaert & Overlord Brecht Kets
// http://www.digitalartsandentertainment.com/
//------------------------------------------------------------------------------------------------------
//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "MeshFilterLoader.h"
#include "..\Helpers\BinaryReader.h"
#include "..\Helpers\EffectHelper.h"
#include <algorithm>

#define OVM_vMAJOR 1
#define OVM_vMINOR 1

MeshFilter* MeshFilterLoader::LoadContent(const wstring& assetFile)
{
	auto binReader = new BinaryReader();
	binReader->Open(assetFile);

	if (!binReader->Exists())
		return nullptr;

	//READ OVM FILE
	int versionMajor = binReader->Read<char>();
	int versionMinor = binReader->Read<char>();

	if (versionMajor != OVM_vMAJOR || versionMinor != OVM_vMINOR)
	{
		Logger::LogFormat(LogLevel::Warning, L"MeshDataLoader::Load() > Wrong OVM version\n\tFile: \"%s\" \n\tExpected version %i.%i, not %i.%i.", assetFile.c_str(), OVM_vMAJOR, OVM_vMINOR, versionMajor, versionMinor);

		delete binReader;
		return nullptr;
	}

	UINT vertexCount = 0;
	UINT indexCount = 0;

	auto pMesh = new MeshFilter();

	for (;;)
	{
		auto meshDataType = (MeshDataType)binReader->Read<char>();
		if (meshDataType == MeshDataType::END)
			break;

		auto dataOffset = binReader->Read<UINT>();

		switch (meshDataType)
		{
		case MeshDataType::HEADER:
		{
			pMesh->m_MeshName = binReader->ReadString();
			vertexCount = binReader->Read<UINT>();
			indexCount = binReader->Read<UINT>();

			pMesh->m_VertexCount = vertexCount;
			pMesh->m_IndexCount = indexCount;
		}
		break;
		case MeshDataType::POSITIONS:
		{
			pMesh->m_HasElement |= ILSemantic::POSITION;

			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT3 pos;
				pos.x = binReader->Read<float>();
				pos.y = binReader->Read<float>();
				pos.z = binReader->Read<float>();
				pMesh->m_Positions.push_back(pos);
			}
		}
		break;
		case MeshDataType::INDICES:
		{
			for (UINT i = 0; i < indexCount; ++i)
			{
				pMesh->m_Indices.push_back(binReader->Read<DWORD>());
			}
		}
		break;
		case MeshDataType::NORMALS:
		{
			pMesh->m_HasElement |= ILSemantic::NORMAL;

			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT3 normal;
				normal.x = binReader->Read<float>();
				normal.y = binReader->Read<float>();
				normal.z = binReader->Read<float>();
				pMesh->m_Normals.push_back(normal);
			}
		}
		break;
		case MeshDataType::TANGENTS:
		{
			pMesh->m_HasElement |= ILSemantic::TANGENT;

			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT3 tangent;
				tangent.x = binReader->Read<float>();
				tangent.y = binReader->Read<float>();
				tangent.z = binReader->Read<float>();
				pMesh->m_Tangents.push_back(tangent);
			}
		}
		break;
		case MeshDataType::BINORMALS:
		{
			pMesh->m_HasElement |= ILSemantic::BINORMAL;

			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT3 binormal;
				binormal.x = binReader->Read<float>();
				binormal.y = binReader->Read<float>();
				binormal.z = binReader->Read<float>();
				pMesh->m_Binormals.push_back(binormal);
			}
		}
		break;
		case MeshDataType::TEXCOORDS:
		{
			pMesh->m_HasElement |= ILSemantic::TEXCOORD;

			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT2 tc;
				tc.x = binReader->Read<float>();
				tc.y = binReader->Read<float>();
				pMesh->m_TexCoords.push_back(tc);
			}
		}
		break;
		case MeshDataType::COLORS:
		{
			pMesh->m_HasElement |= ILSemantic::COLOR;

			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT4 color;
				color.x = binReader->Read<float>();
				color.y = binReader->Read<float>();
				color.z = binReader->Read<float>();
				color.w = binReader->Read<float>();
				pMesh->m_Colors.push_back(color);
			}
		}
		break;
		case MeshDataType::BLENDINDICES:
		{
			pMesh->m_HasElement |= ILSemantic::BLENDINDICES;

			//Start parsing the BlendIndices for every vertex
			//and add them to the corresponding vector
			//pMesh->m_BlendIndices
			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT4 blendIndices;
				blendIndices.x = binReader->Read<float>();
				blendIndices.y = binReader->Read<float>();
				blendIndices.z = binReader->Read<float>();
				blendIndices.w = binReader->Read<float>();
				pMesh->m_BlendIndices.push_back(blendIndices);
			}
		}
		break;
		case MeshDataType::BLENDWEIGHTS:
		{
			pMesh->m_HasElement |= ILSemantic::BLENDWEIGHTS;

			//Start parsing the BlendWeights for every vertex
			//and add them to the corresponding vector
			//pMesh->m_BlendWeights
			for (UINT i = 0; i < vertexCount; ++i)
			{
				XMFLOAT4 blendWeights;
				blendWeights.x = binReader->Read<float>();
				blendWeights.y = binReader->Read<float>();
				blendWeights.z = binReader->Read<float>();
				blendWeights.w = binReader->Read<float>();
				pMesh->m_BlendWeights.push_back(blendWeights);
			}
		}
		break;
		case MeshDataType::ANIMATIONCLIPS:
		{
			pMesh->m_HasAnimations = true;

			//Start parsing the AnimationClips
			//1. Read the clipCount
			USHORT clipCount = binReader->Read<USHORT>();
			pMesh->m_AnimationClips.reserve(clipCount);

			//2. For every clip
			for (size_t j = 0; j < clipCount; j++)
			{
				//3. Create a AnimationClip object (clip)
				AnimationClip clip;

				//4. Read/Assign the ClipName
				clip.Name = binReader->ReadString();

				//5. Read/Assign the ClipDuration
				clip.Duration = binReader->Read<float>();

				//6. Read/Assign the TicksPerSecond
				clip.TicksPerSecond = binReader->Read<float>();

				//7. Read the KeyCount for this clip
				USHORT keyCount = binReader->Read<USHORT>();
				clip.Keys.reserve(keyCount);

				//8. For every key
				for (size_t k = 0; k < keyCount; k++)
				{
					//9. Create a AnimationKey object (key)
					AnimationKey key;
					
					//10. Read/Assign the Tick
					key.Tick = binReader->Read<float>();

					//11. Read the TransformCount
					USHORT transformCount = binReader->Read<USHORT>();

					key.BoneTransforms.reserve(transformCount);
					//12. For every transform
					for (size_t l = 0; l < transformCount; l++)
					{
						//13. Create a XMFLOAT4X4
						XMFLOAT4X4 transform;

						//14. The following 16 floats are the matrix values, they are stored by row
						transform = binReader->Read<XMFLOAT4X4>();

						//MATRIX:
						// [ float0	float1	float2	float3 ]
						// [ float4	...		...		...	   ]
						// [ ...	...		...		...	   ]
						// [ ...	...		...		float15]

						// Debug info:
						//XMMATRIX transformM = XMLoadFloat4x4(&transform);
						//XMVECTOR scaleV, rotateV, translateV;
						//XMMatrixDecompose(&scaleV, &rotateV, &translateV, transformM);
						//
						//XMFLOAT4 scale, rotate, translate;
						//XMStoreFloat4(&scale, scaleV);
						//XMStoreFloat4(&rotate, rotateV);
						//XMStoreFloat4(&translate, translateV);
						//XMFLOAT3 rotateEuler = QuaternionToEuler(rotate);
						//
						//Logger::LogFormat(LogLevel::Info, L"translate: %0.2f, %0.2f, %0.2f,   rotate: %0.2f, %0.2f, %0.2f, scale: %0.2f, %0.2f, %0.2f",
						//	translate.x, translate.y, translate.z, rotateEuler.x, rotateEuler.y, rotateEuler.z, scale.x, scale.y, scale.z);

						//15. Add The matrix to the BoneTransform vector of the key
						key.BoneTransforms.push_back(transform);
					}

					//16. Add the key to the key vector of the clip
					clip.Keys.push_back(key);
				}
					
				//17. Add the clip to the AnimationClip vector of the MeshFilter (pMesh->m_AnimationClips)
				pMesh->m_AnimationClips.push_back(clip);
			}
		}
		break;
		case MeshDataType::SKELETON:
		{
			//1. Read/Assign the boneCount (pMesh->m_BoneCount)
			pMesh->m_BoneCount = binReader->Read<USHORT>();

			//2. Move the buffer to the next block position (don't forget that we already moved the reader ;) )
			binReader->MoveBufferPosition(dataOffset - sizeof(pMesh->m_BoneCount));
		}
		break;
		default:
			binReader->MoveBufferPosition(dataOffset);
			break;
		}
	}

	delete binReader;

	return pMesh;
}

void MeshFilterLoader::Destroy(MeshFilter* objToDestroy)
{
	SafeDelete(objToDestroy);
}
