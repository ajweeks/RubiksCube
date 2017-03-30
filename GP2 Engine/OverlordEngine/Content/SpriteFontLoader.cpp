#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "../Helpers/BinaryReader.h"
#include "../Content/ContentManager.h"
#include "../Graphics/TextRenderer.h"

#include <cstdint>


SpriteFontLoader::SpriteFontLoader()
{

}


SpriteFontLoader::~SpriteFontLoader()
{
}

#pragma warning( disable : 4702 )
SpriteFont* SpriteFontLoader::LoadContent(const wstring& assetFile)
{
	auto pBinReader = new BinaryReader(); //Prevent memory leaks
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());

		return nullptr;
	}

	//Parse the Identification bytes (B,M,F)
	const char fileID1 = pBinReader->Read<int8_t>();
	const char fileID2 = pBinReader->Read<int8_t>();
	const char fileID3 = pBinReader->Read<int8_t>();
	if (fileID1 != 'B' || fileID2 != 'M' || fileID3 != 'F')
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"Couldn't find .fnt file! File path: %s", assetFile.c_str());
		return nullptr;
	}

	//Parse the version (version 3 required)
	char fileVersion = pBinReader->Read<int8_t>();
	if (fileVersion != 3)
	{
		delete pBinReader;
		Logger::LogFormat(LogLevel::Error, L"Inavlid .fnt file! Must be version 3. File path: %s, "
			" file verison: %i", assetFile.c_str(), fileVersion);
		return nullptr;
	}
	
	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	int blockType = pBinReader->Read<int8_t>();
	int blockSize = pBinReader->Read<int32_t>();
	
	pSpriteFont->m_FontSize = pBinReader->Read<int16_t>();

	//Move the binreader to the start of the FontName
	pBinReader->MoveBufferPosition(12);

	//Retrieve the FontName [SpriteFont::m_FontName]
	pSpriteFont->m_FontName = pBinReader->ReadNullString();


	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockType = pBinReader->Read<int8_t>();
	blockSize = pBinReader->Read<int32_t>();

	// Unused values
	const int lineHeight = pBinReader->Read<uint16_t>();
	const int base = pBinReader->Read<uint16_t>();

	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	pSpriteFont->m_TextureWidth = pBinReader->Read<uint16_t>();
	pSpriteFont->m_TextureHeight = pBinReader->Read<uint16_t>();

	//Retrieve PageCount
	const int pageCount = pBinReader->Read<uint16_t>();
	if (pageCount > 1)
	{ 
		Logger::LogFormat(LogLevel::Error, L"Sprite font has more than one page! File path: %s", assetFile.c_str());
	}
	
	//Advance to Block2 (Move Reader)
	pBinReader->MoveBufferPosition(5);
	

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockType = pBinReader->Read<int8_t>();
	blockSize = pBinReader->Read<int32_t>();
	
	//Retrieve the PageName (store Local)
	const std::wstring pageName = pBinReader->ReadNullString();
	
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	if (pageName.empty())
	{
		Logger::LogFormat(LogLevel::Error, L"Invalid sprite font! Contains empty page. File path: %s", assetFile.c_str());
	}

	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)

	const size_t lastSlash = assetFile.rfind(L'/') + 1;
	const wstring directoryPath = assetFile.substr(0, lastSlash);

	const wstring pagePath = directoryPath + pageName;
		
	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(pagePath);

	//**********
	// BLOCK 4 *
	//**********
	//Retrieve the blockId and blockSize
	blockType = pBinReader->Read<int8_t>();
	blockSize = pBinReader->Read<int32_t>();

	//Retrieve Character Count (see documentation)
	int charCount = blockSize / 20;
	
	for (int i = 0; i < charCount; i++)
	{
		//Retrieve Every Character, For every Character:

		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		const wchar_t charID = (wchar_t)pBinReader->Read<int32_t>();
		if (charID == L'.')
		{
			int x = 1;
			++x;
		}

		//> Check if CharacterId is valid (SpriteFont::IsCharValid), 
		// Log Warning and advance to next character if not valid
		if (!SpriteFont::IsCharValid(charID))
		{
			Logger::LogFormat(LogLevel::Warning, L"Invalid character id found in sprite font file. Path: %s", assetFile.c_str());
			continue;
		}

		//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
		FontMetric& metric = pSpriteFont->GetMetric(charID);

		//> Set IsValid to true [FontMetric::IsValid]
		metric.IsValid = true;
		
		//> Set Character (CharacterId) [FontMetric::Character]
		metric.Character = charID;

		//> Retrieve Xposition (store Local)
		int x = pBinReader->Read<int16_t>();

		//> Retrieve Yposition (store Local)
		int y = pBinReader->Read<int16_t>();

		//> Retrieve & Set Width [FontMetric::Width]
		metric.Width = pBinReader->Read<int16_t>();

		//> Retrieve & Set Height [FontMetric::Height]
		metric.Height = pBinReader->Read<int16_t>();

		//> Retrieve & Set OffsetX [FontMetric::OffsetX]
		metric.OffsetX = pBinReader->Read<int16_t>();

		//> Retrieve & Set OffsetY [FontMetric::OffsetY]
		metric.OffsetY = pBinReader->Read<int16_t>();

		//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
		metric.AdvanceX = pBinReader->Read<int16_t>();

		//> Retrieve & Set Page [FontMetric::Page]
		metric.Page = pBinReader->Read<int8_t>();

		//> Retrieve Channel (BITFIELD!!!) 
		//	> See documentation for BitField meaning [FontMetrix::Channel]
		int channel = pBinReader->Read<int8_t>();
		metric.Channel = (channel == 1 ? 2 : (channel == 2 ? 1 : (channel == 4 ? 0 : 3)));

		//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
		float u = (x / (float)pSpriteFont->m_TextureWidth);
		float v = (y / (float)pSpriteFont->m_TextureHeight);
		metric.TexCoord = XMFLOAT2(u, v);
	}

	delete pBinReader;
	return pSpriteFont;

#pragma warning(default:4702)  
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}