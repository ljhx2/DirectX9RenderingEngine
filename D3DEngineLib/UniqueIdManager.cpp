#include "UniqueIdManager.h"

namespace ENGINE
{

	UniqueIdManager::UniqueIdManager()
	{
		UniqueID = 0;
	}


	UniqueIdManager::~UniqueIdManager()
	{
	}

	unsigned int UniqueIdManager::CreateUID()
	{
		//if (UniqueID == UINT_MAX)
		//{
		//
		//}

		return ++UniqueID;
	}
}