#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//====== Thread Local Storage (TLS) =====

// TLS�� Thread�� ������ �ִ� ������ ���� ����.
// �����ϴ� �����Ͱ� ���� ��, �ٸ� Thread��� ������ ���� ����ϴ� ��Ȳ�� ��.
// �׷��� �ڽ��� ����� �����͸� TLS�� �����Ͽ� ����� �� �ֱ� ������, ���ʿ��� ������ �� �ʿ� ����.
// Thread �� ������ �ְ�, Thread�� ���� ������ ���� �޸� ����.

// Multi-Thread ���� �ϰ��� ����� �߿���
// ����� �� ���� ������ Thread�� ���� ������ �ʿ������� ����.

//__declspec(thread) int32 value;	// C++ 11���� ��� ���.
thread_local int32 LThreadId = 0;	// ������ thread�� ���� ������ ����. (TLS ����)

void ThreadMain(int32 threadId)
{
	LThreadId = threadId;

	while (true)
	{
		cout << "Hi! I am Thread " << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int TLS_main()
{
	vector<thread> threads;

	for (int32 i = 0; i < 10; ++i)
	{
		int32 threadId = i + 1;
		threads.emplace_back(thread(ThreadMain, threadId));
	}

	for (thread& t : threads)
		t.join();

	return 0;
}