#pragma once
#include "..\Diagnostics\Logger.h"

class BinaryReader
{
public:
	BinaryReader(void);
	~BinaryReader(void);


	template<class T>
	T Read()
	{
		if(m_pReader==nullptr)
		{
			Logger::LogError(L"BinaryReader doesn't exist!\nUnable to read binary data...");
			return T();
		}

		T value;
		m_pReader->read((char*)&value, sizeof(T));
		return value;
	}

	wstring ReadString();
	wstring ReadLongString();
	wstring ReadNullString();

	int GetBufferPosition();
	bool SetBufferPosition(int pos);
	bool MoveBufferPosition(int move);
	bool Exists() const { return m_Exists; }

	void Open(wstring binaryFile);
	void Open(char* s, UINT32 size);
	void Close();

private: 

	bool m_Exists;
	istream* m_pReader;

private:
	// -------------------------
	// Disabling default copy constructor and default 
	// assignment operator.
	// -------------------------
	BinaryReader(const BinaryReader& yRef);									
	BinaryReader& operator=(const BinaryReader& yRef);
};

