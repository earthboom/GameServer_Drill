#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

//===== Condition Variable (조건 변수) ======
// C++ 11 의 표준
// Linux, Window 동시 사용 가능
// Event보다 condition_variable 사용을 추천

mutex cv_m;
queue<int32> cv_q;
HANDLE cv_handle;

// mutex header에 포함.
// 표준 mutex 짝지어 동작
// 참고 - condition_variable은 User-Level Object(커널 오브젝트가 아님)
// cv는 Event와 매우 유사.
// (커널 레벨 오브젝트는 다른 프로그램끼리도 서로 이벤트를 이용해 동기화하여 사용이 가능.)
// (유저 레벨 오브젝트는 동일한 프로그램 내부에서만 사용이 가능.)
condition_variable cv;

// 좀더 일반적인 상황으로 확장하고 싶다면
//#include <condition_variable>
//condition_variable_any cv_any;

void cv_Producer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 공유 변수값을 수정
		// 3) Lock을 풀고
		// 4) 조건 변수(CV)를 통해 다른 Thread에게 통지

		{
			unique_lock<mutex> lock(cv_m);	// Lock을 잡고(1)
			cv_q.emplace(100);				// 공유 변수값을 수정(2)
		}									// 범위를 벗어나며 Lock이 풀리고(3)

		// 조건 변수(CV)를 통해 다른 Trhead에게 통지(4)
		cv.notify_one();					// wait중인 thread가 있으면 딱 1개만 깨운다.

		//cv.notify_all();					// 모든 Thread에게 통지

		//this_thread::sleep_for(10000ms);	// 100ms = 0.1초
	}
}

void cv_Consumer()
{
	while (true)
	{
		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족 O -> 빠져나와 이어서 코드 진행
		// - 만족 X -> Lock을 풀어주고 대기 상태로 전환

		unique_lock<mutex> lock(cv_m); // Lock을 잡고 (1)

		// 조건 확인 (2)
		// cv.wait의 첫번째 매개변수의 타입은 unique_lock.
		// CV에서 조건을 만족할 경우, 중간에 Lock을 해제해야 하기 때문에, 유연한 unique_lock을 사용.
		// cv.wait의 두번째 매개변수는 함수 포인트 or 람다로 깨어날 조건을 대입함.
		cv.wait(lock, []() {return cv_q.empty() == false; });


		// 그런데 notify_one을 했으면 항상 조건식을 만족하는 것이 아닐까?
		// 이런 상황을 Spurious Wakeup(가짜 기상?)
		// notify_one을 할 때, Lock을 항상 잡고 있는 것이 아니기 때문.


		//while (cv_q.empty() == false)
		{
			int32 data = cv_q.front();
			cv_q.pop();
			cout << cv_q.size() << endl;
		}
	}
}

int condition_variable_main()
{
	thread t1(cv_Producer);
	thread t2(cv_Consumer);

	t1.join();
	t2.join();

	return 0;
}