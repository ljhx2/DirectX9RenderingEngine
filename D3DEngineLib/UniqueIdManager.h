#pragma once

#define g_pUIDManager UniqueIdManager::GetInstance()

namespace ENGINE
{
	class UniqueIdManager
	{
	private:
		UniqueIdManager();
		~UniqueIdManager();

	public:
		static UniqueIdManager* GetInstance() { static UniqueIdManager instance; return &instance; }

	private:
		unsigned int UniqueID;


	public:
		unsigned int CreateUID();

	};

}