//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "PhysxErrorCallback.h"
#include <string>
#include <sstream>
#include "../Diagnostics/Logger.h"
using namespace std;

PhysxErrorCallback::PhysxErrorCallback(void){}
PhysxErrorCallback::~PhysxErrorCallback(void){}

void PhysxErrorCallback::reportError(PxErrorCode::Enum code , const char* message, const char* file, int line)
{
	wstringstream ss;
	ss<<L"PHYSX CALLBACK REPORT:"<<endl;
	ss<<"Error Code: "<<code<<endl;
	ss<<"Message: "<<message<<endl;
	ss<<"File: "<<file<<" (line: "<<line<<")";

	switch(code)
	{
	case PxErrorCode::eDEBUG_INFO:
		Logger::LogInfo(ss.str());
		break;
	case PxErrorCode::ePERF_WARNING:
	case PxErrorCode::eDEBUG_WARNING:
			Logger::LogWarning(ss.str());
			break;
	default:
		Logger::LogError(ss.str());
		break;
	}
}