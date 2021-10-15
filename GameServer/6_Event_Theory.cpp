#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

//===== Event ======
// Kernel 관리자에게 원하는 메모리의 lock이 풀렸음을 알려달라고 하는 것 (Event)
// event를 통해서, 잠든 Thread를 꺠우는 방식이라 매우 효율적
// Kernel mode에서 Event를 생성하는 무거운 작업이라 남발해선 안된다.

// Auto Reset Event
// - 문이 자동으로 잠김

//Manual Reset Event
// - 문이 수동으로 잠김

mutex event_m;
queue<int32> event_q;
HANDLE event_handle;

void event_Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(event_m);
			event_q.emplace(100);
		}

		// 해당 핸들을 가진 Kernel Object의 상태를 Signal(파란불)상태로 변경해달라고 요청 (true)
		::SetEvent(event_handle);

		this_thread::sleep_for(10000ms);	// 100ms = 0.1초
	}
}

void event_Consumer()
{
	// event_q에 데이터를 넣어주는 시간이 엄청나게 길어지면, 이 함수의 대부분의 작업이 잉여작업이 되어 버림.
	// 결국 이렇게 반복적인 작업이 쓸데없는 CPU를 점유하게 되어 버림.
	// 따라서, event_q에 데이터를 넣어줄 때만 동작하도록 하는 Event를 만들어 준다.

	while (true)
	{
		// 해당 핸들을 가진 Kernel Object의 상태가 Signal(파란불)상태가 될떄까지 대기.
		// Non-Signal(빨간불)이면 두번째 매개변수의 값만큼 해당 위치에서 thread가 대기함
		::WaitForSingleObject(event_handle, INFINITE);

		// 해당 핸들의 Kernel Object의 bManualReset가 FALSE이면 Auto Reset이기 때문에 
		// 자동적으로 Non-Signal 상태로 변경 된다.

		// TRUE이면 Manual Reset이어서, 추가적으로 강제로 Non-Signal 상태로 만들어 줘야 한다.
		//::ResetEvent(event_handle);

		unique_lock<mutex> lock(event_m);
		if (event_q.empty() == false)
		{
			int32 data = event_q.front();
			event_q.pop();
			cout << data << endl;
		}
	}
}

// Event를 활용하면 매우 효율적이나, Kernel level에서의 작업이 추가되어 무겁다.
// 따라서, 가끔 일어나는 작업이면 모르나, 매우 빈번하게 일어나는 작업에 이용하면, 오히려 프로그램을 무겁게 할 수 있다.
// Spin Lock, Sleep 은 User level에서 동작하기 때문에 보다 가볍기 때문에, 빈번한 작업에 오히려 적합할 수 있다.

int Event_main()
{
	// HANDLE : 일종의 Event를 구별하기 위한 정수값

	//Window API
	// Kernel에서 만들어줌, 그래서 "Kernel Object"임
	// Useage Count : 해당 Kernel Object를 몇명이 사용하고 있는가?
	// Signal(파란불, true) / Non-Signal(빨간불, false) : Kernel Object가 가진 두 가지 상태 (boolean)
	// Auto / Manual : boolean
	event_handle = ::CreateEvent(
		NULL/*보안속성*/,
		FALSE/*bManualReset(TRUE : Manual Reset | FALSE : Auto Reset)*/,
		FALSE/*bInitialState(Event의 초기 상태)*/,
		NULL/*Event 이름*/
	);

	thread t1(event_Producer);
	thread t2(event_Consumer);

	t1.join();
	t2.join();

	::CloseHandle(event_handle);

	return 0;
}