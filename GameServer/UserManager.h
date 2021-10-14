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
		
		//���� �������� ����
		
		return nullptr;
	}

	void ProcessSave();	// �ش� ������ ������ �����ϴ� �۾�

private:
	mutex _mutex;
};

