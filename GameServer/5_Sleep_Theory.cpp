#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>


//===== Sleep ======
// �����Ϸ��� ���� �̹� �����Ǿ� ���� ��, ���� �� ����ϴ� ���� �ƴ϶�, �������� �ٽ� lock�� ��û�ϴ� ���

// �����ٷ�, ���ؽ�Ʈ ����Ī�� ���� ������ �˾ƺ���.

class SpinLock_sleep
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)
		bool expected = false;	// ���� ���� ��Ȳ (Before)
		bool desired = true;	// �ٲ�� ���ϴ� ������ ��Ȳ (After)

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;

			// C++ 11 ���Ŀ� thread���� sleep ���� �Լ��� ������ ( System call )
			//this_thread::sleep_for(std::chrono::milliseconds(100));	// �������� ������ ������
			//this_thread::sleep_for(100ms);	// 100ms == std::chrono::milliseconds(100) ���� ǥ����.


			//this_thread::yield();	// �����ٷ����� ���� time slice�� �����Ѵٴ� �ǹ� ( == this_thread::sleep_for(0ms))
			this_thread::sleep_for(0ms);

			// ������ �����ؾ� �ϴ� �κп����� System Call�� �ִ��� �����ؾ� ��. �̹� Sleep�� �� ���.
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