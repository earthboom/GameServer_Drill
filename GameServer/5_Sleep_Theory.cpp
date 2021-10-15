#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>


//===== Sleep ======
// 점유하려던 곳이 이미 점유되어 있을 때, 무한 정 대기하는 것이 아니라, 랜덤으로 다시 lock을 요청하는 방식

// 스케줄러, 컨텍스트 스위칭에 대한 개념을 알아보자.

class SpinLock_sleep
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)
		bool expected = false;	// 현재 예상 상황 (Before)
		bool desired = true;	// 바뀌길 원하는 나중의 상황 (After)

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			// C++ 11 이후에 thread에서 sleep 관련 함수를 제공함 ( System call )
			//this_thread::sleep_for(std::chrono::milliseconds(100));	// 언제까지 잘지를 정해줌
			//this_thread::sleep_for(100ms);	// 100ms == std::chrono::milliseconds(100) 같은 표현임.


			//this_thread::yield();	// 스케줄러에게 받은 time slice를 포기한다는 의미 ( == this_thread::sleep_for(0ms))
			this_thread::sleep_for(0ms);

			// 빠르게 동작해야 하는 부분에서는 System Call을 최대한 자제해야 함. 이번 Sleep이 그 경우.
		}
	}

	void unlock()
	{
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32 sleep_sum = 0;
mutex sleep_m;
SpinLock_sleep spinLock_sleep;

void sleep_Add()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock_sleep> guard(spinLock_sleep);
		sleep_sum++;
	}
}

void sleep_Sub()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock_sleep> guard(spinLock_sleep);
		sleep_sum--;
	}
}

int Sleep_main()
{
	thread t1(sleep_Add);
	thread t2(sleep_Sub);

	t1.join();
	t2.join();

	cout << sleep_sum << endl;
	return 0;
}