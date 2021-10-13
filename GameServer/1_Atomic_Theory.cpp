#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>	// Window, Linux 상관없이 범용적인 Thread 생성이 가능 (C++ 11 부터)
#include <atomic>	// atomic : All-Or-Nothing. (한 번에 전부 실행되거나 되지 않거나) (Window, Linux 동시 사용 가능)

//int32 sum = 0;	// 전역 변수는 Data 영역에 올라감, 따라서 모든 thread가 공유할 수 있는 데이터

// 이 Atomic 변수를 두고 Thread 들이 경합을 벌이면
// 해당 변수를 가지고 작업중인 Thread가 있으면, 다른 Thread들은 짧게 대기하게 됨.
// 그 대기를 하게 하는 것이 CPU
// 단점 : atomic 변수들로 계산이 생각보다 무거움. 무조건적인 사용은 지양.
atomic<int32> sum = 0;

// Add(), Sub()를 각각의 thread로 동시에 실행될 경우.
// 공용 데이터 sum에 값이 덮어 쓰여지며, 값을 예측할 수 없게 되어버림.
// 이를 해결하기 위한 것이 동기화
void Add()
{
	//함수 내에 선언된 변수는 Stack 영역에 올라감.
	//다른 thread의 영향을 받지 않음.
	for (int32 i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(1);
		//sum++;

		// sum++ 실행을 단계별로 표현(eax는 CPU의 레지스터)
		//int32 eax = sum;
		//eax = eax + 1;
		//sum = eax;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(-1);
		//sum--;

		// sum-- 실행을 단계별로 표현(eax는 CPU의 레지스터)
		//int32 eax = sum;
		//eax = eax - 1;
		//sum = eax;
	}
}

int atomic_main()
{
	Add();
	Sub();

	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();

	cout << sum << endl;

	return 0;
}
