#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//====== Thread Local Storage (TLS) =====

// TLS는 Thread가 가지고 있는 별도의 저장 공간.
// 공유하는 데이터가 있을 때, 다른 Thread들과 경합이 심해 대기하는 상황이 옮.
// 그러나 자신이 사용할 데이터를 TLS에 저장하여 사용할 수 있기 때문에, 불필요한 경합을 할 필요 없음.
// Thread 가 가지고 있고, Thread만 접근 가능한 전역 메모리 공간.

// Multi-Thread 에선 일감의 배분이 중요함
// 배분이 잘 되지 않으면 Thread들 간의 경합이 필연적으로 생김.

//__declspec(thread) int32 value;	// C++ 11이전 사용 방법.
thread_local int32 LThreadId = 0;	// 각각의 thread만 접근 가능한 변수. (TLS 변수)

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