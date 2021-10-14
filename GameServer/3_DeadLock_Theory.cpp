#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include "AccountManager.h"
#include "UserManager.h"

// Dead Lock

// 서로 교차적으로 lock을 걸고 있는 상황.
// 양쪽 Thread들이 교착상태에 빠져서, 무한정 대기 상태에 빠진 상황. ( = Dead Lock )
void Func1()
{
	for (int32 i = 0; i < 100; ++i)
	{
		UserManager::instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 100; ++i)
	{
		AccountManager::instance()->ProcessLogin();
	}
}

int DeadLock_main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	//참고
	mutex m1;
	mutex m2;

	// 일관적인 순서에 따라 lock을 검.
	std::lock(m1, m2);	// m1.lock(), m2.lock()

	// adopt_lock : 이미 lock된 상태이니까, 나중에 소멸될 떄 풀어주기만 하라.
	lock_guard<mutex> g1(m1, std::adopt_lock);
	lock_guard<mutex> g1(m2, std::adopt_lock);

	return 0;
}