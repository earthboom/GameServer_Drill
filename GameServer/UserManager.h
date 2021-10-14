#pragma once

#include <mutex>

class User
{
	//TODO
};

class UserManager
{
public:
	static UserManager* instance()
	{
		static UserManager instance;
		return &instance;
	}

	User* GetUser(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		
		//뭔가 가져오는 동작
		
		return nullptr;
	}

	void ProcessSave();	// 해당 유저의 정보를 저장하는 작업

private:
	mutex _mutex;
};

