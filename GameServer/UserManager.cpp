#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// ���ÿ� Lock�� �ɸ��� ���.
	// user Lock
	lock_guard<mutex> guard(_mutex);

	// account Lock
	Account* account = AccountManager::instance()->GetAccount(100);

	// TODO
}
