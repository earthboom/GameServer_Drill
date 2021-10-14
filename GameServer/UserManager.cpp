#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// 동시에 Lock이 걸리는 경우.
	// user Lock
	lock_guard<mutex> guard(_mutex);

	// account Lock
	Account* account = AccountManager::instance()->GetAccount(100);

	// TODO
}
