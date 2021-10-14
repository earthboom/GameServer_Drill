#pragma once

#include <mutex>

class Account
{
	// TODO
};

// 계정 관리 ( 임시 )
class AccountManager
{
public:
	static AccountManager* instance()
	{
		static AccountManager instance;
		return &instance;
	}

	Account* GetAccount(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		
		// 뭔가를 가져오는 동작

		return nullptr;
	}

	void ProcessLogin();	// Login 과정

private:
	mutex _mutex;
	//map<int32, Account*> _accounts;
};

