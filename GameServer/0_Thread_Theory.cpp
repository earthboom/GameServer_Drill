#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>	// Window, Linux 상관없이 범용적인 Thread 생성이 가능 (C++ 11 부터)

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
	cout << num << endl;
}

int thread_main()
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

	return 0;
}
