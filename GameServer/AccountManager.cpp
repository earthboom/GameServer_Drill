#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

// �̸� �����ϱ� ���ؼ� mutex�� class�� �����Ͽ� id�� �ű�� ����� ������ ����

// mutex�� �����ϴ� Manager�� �����Ͽ�, graph �˰����� ������ 
// 1->2->3ó�� ������ �ƴ� 1->2->1 �� ���� ����Ŭ�� �Ͼ�� ��������� �����Ͽ� �����ϴ� ���

void AccountManager::ProcessLogin()
{
	// ���ÿ� Lock�� �ɸ��� ����.
	
	// Dead Lock ���¸� �ذ��ϱ� ���� ��ġ ����.
	// ��� ��쿡 �̷��� ������ ���ߴ� ���� ���� ���� �ƴ�.
	User* user = UserManager::instance()->GetUser(100);

	// account Lock
	lock_guard<mutex> guard(_mutex);

	// user Lock ( Dead Lock �� �ɸ��� ��ġ )
	// �ٸ� Thread�� �̹� UserManager�� ProcessSave�� ȣ���Ͽ� 
	// UserManager�� mutex�� Lock�� �ɾ��� ������, ������ ���� ���� ������ ��� ����.
	// �̴� UserManager�� ProcessSave�� �������� �ٸ� Thread�� ����������. (��������)
	//User* user = UserManager::instance()->GetUser(100);

	// TODO
}
