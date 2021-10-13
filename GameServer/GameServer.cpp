#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>	// Window, Linux 상관없이 범용적인 Thread 생성이 가능 (C++ 11 부터)
#include <atomic>	// atomic : All-Or-Nothing. (한 번에 전부 실행되거나 되지 않거나) (Window, Linux 동시 사용 가능)


int main()
{
	return 0;
}


//=========================
//===== Create Atomic =====
//=========================
/*
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

int main()
{
	Add();
	Sub();
	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;
}
*/

//=========================
//===== Create Thread =====
//=========================
/*
void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
	cout << num << endl;
}

int main()
{
	//HelloThread(); //MainThread로 실행

	vector<std::thread> v;

	// 한번에 여러 Thread를 사용할 경우, 각자 병렬로 실행됨.
	// 따라서 동일한 작업에 한에서 어떤 Thread의 동작이 느리고 빠른지를 예상할 수 없음.
	for (int32 i = 0; i < 10; ++i)
	{
		v.emplace_back(std::thread(HelloThread_2, i));	// 호출할 함수에 매개변수가 있을 시에.
	}

	for (int32 i = 0; i < 10; ++i)
	{
		if (v[i].joinable())
			v[i].join();
	}


	// Thread 생성
	// - OS 에 요청.
	//std::thread t;	// thread가 동작하지는 않음.

	//auto id1 = t.get_id(); // 동작하지 않기 때문에 0 반환

	//t = std::thread(HelloThread); // 생성된 Thread가 호출

	//int32 count = t.hardware_concurrency();	// CPU 코어 개수? 동시에 실행될 수 있는 코어 개수를 의미. (정확하지는 않다고 함.)

	//auto id2 = t.get_id();	// Thread마다의 ID

	// 만들어진 Thread 객체 t와 실질적으로 구동된 Thread와의 연결고리를 끊어주는 의미.
	// BackGround Thread로 독립적으로 움직이는 Thread가 되는 것.
	// 더 이상 해당 thread에 대한 정보를 추출할 수 없어짐.
	// 웬만하면 잘 사용하지 않음.
	//t.detach();	// std::thread 객체에서 실제 Thread를 분리

	// 실질적으로 연동된 Thread가 없다는 것을 판별하기 위한 함수.
	// 현재 thread가 동작하는가 안하는가를 판별하기 위함.
	//if (t.joinable())
	//{
	//	t.join();	// Thread t가 종료될 떄까지 main thread가 대기하도록 함 ( Join() )
	//}

	cout << "Hello Main" << endl;
	// 생성된 thread가 동작 중인데, main Thread가 먼저 종료되면 오류 발생하기 때문에 대기해줘야 함.
}
*/
