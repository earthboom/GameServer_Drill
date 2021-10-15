#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

//===== Spin Lock ======
// 경합에 져서, 대기 중일 때, 무한정 대기상태로 Lock을 요청하고 있는 상태라고 생가하면 됨.

// while문을 통해 User level에서만 동작하기 때문에, Context Switching을 하징 않아 무겁지 않음.



// 여러 Thread가 메모리에 접근하고 자물쇠(Lock)을 얻었다고 동시에 주장하는 경우가 있음.
// 
// Thread가 메모리를 점유하고 자물쇠(Lock)을 얻는 과정이 분리되어선 안되고,
// 그렇게 안될거면, 아예 접근조차도 할 수 없게해야 함 (Atomic한 동작)

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)

		bool expected = false;	// 현재 예상 상황 (Before)
		bool desired = true;	// 바뀌길 원하는 나중의 상황 (After)

		// CAS 의사코드
		//if (_locked == expected)
		//{
		//	expected = _locked;
		//	_locked = desired;
		//	return true;
		//}
		//else
		//{
		//	expected = _locked;
		//	return false;
		//}

		// atomic 함수인 compare_exchange_strong은 위의 CAS 의사코드를 한 번에 실행함. (atomic한 동작)
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			// 무한 루프를 돌면서 계속 시도하는 것.
			// 해당 Thread가 보장 받은 실행 시간을 모두 소진. (Spin Lock 특징)
			// expected 가 true가 될때까지 이게 Spin Lock
			expected = false;
		}




		//while (_locked)
		//{
		//}
		////동시에 접근하여, while문을 그냥 뛰어넘는 경우가 있을 수 있음.
		//_locked = true;
	}

	void unlock()
	{
		//_locked = false;

		_locked.store(false);
	}

private:
	// volatile  ( C++ )
	// 컴파일에게 최적화를 하지 말라는 키워드.
	//volatile bool _locked = false;

	// atomic은 volatile 키워드도 포함하고 있음.
	atomic<bool> _locked = false;
};

int32 spin_sum = 0;
mutex spin_m;
SpinLock spinLock;

void spin_Add()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		spin_sum++;
	}
}

void spin_Sub()
{
	for (int32 i = 0; i < 10'0000; ++i)
	{
		lock_guard<SpinLock> guard(spinLock);
		spin_sum--;
	}
}

int SpinLock_main()
{
	//==================================================
	// volatile 키워드
	// 
	// Release 모드일 경우.
	// 컴파일이 자동으로 최적화하여 int32 a = 4 로 만듦.
	// 여기에 volatile 키워드를 넣으면, 최적화를 수행하지 않음.
	//volatile int32 a = 0;
	//a = 1;
	//a = 2;
	//a = 3;
	//a = 4;
	//cout << a << endl;

	// 기본적으로 flag의 bool 체크가 의미가 없다고 판단함.
	// 일일히 확인하도록 하려면 volatile 키워드를 추가
	//volatile bool flag = true;
	//while (flag)
	//{
	//
	//}
	//==================================================


	thread t1(spin_Add);
	thread t2(spin_Sub);

	t1.join();
	t2.join();

	cout << spin_sum << endl;

	return 0;
}