#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//===== Future ======
// C++11 에서 추가.

int64 future_Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; ++i)
		sum += i;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int Future_main()
{
	// 동기(synchronous) 실행 (일반적)
	// 호출된 함수가 매우 무거운 동작을 하면, main thread가 묶여버리는 현상이 발생할 수 있음.
	// 따라서 다른 thread에 떠넘기는 비동기적 실행이 필요할 때가 있음.
	//int64 sum = future_Calculate();
	//cout << sum << endl;


	// 일반적으로 thread를 만들고, 공용 변수를 생성하지만 이보다 더 가벼운 방식이 있음.
	// 마치 단기 알바와 같은 느낌?

	// std::futrue
	// 언젠가 미래에 결과물을 뱉어줄 것!
	{
		// deferred -> lazy evaluation 지연해서 실행하라.(실제 실행시점을 뒤로 미루는 것.)
		// async -> 별도의 thread를 만들어 실행.(multi-thread)
		// deferred | async -> 둘 중 알아서 골라라.
		std::future<int64> future = std::async(std::launch::async, future_Calculate);

		// TODO

		// std::future_status 는 future 객체의 작업 상태를 알 수 있음.
		//std::future_status status = future.wait_for(1ms);	//대기 함수
		//if (status == future_status::ready)	// 결과물이 준비된 상태.
		//{
		//}


		// 결과물이 필요할 시점
		// deferred 옵션일 경우엔, 이 시점에서 함수가 호출됨.
		// aync 옵션일 경우, thread에서 따로 수행한 결과를 불러옴. 계속 수행 중이라면 대기.
		int64 sum = future.get();

		// class에 속한 맴버함수를 호출할 경우
		//class Knight
		//{
		//public:
		//	int64 GetHp() { return 100; }
		//};
		//
		//Knight knight;
		//std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight);// knight.GetHp()
	}

	// std::promise
	// 미래(std::future)에 결과물을 반환해줄 것이라는 약속(std::promise) - 계약서와 유사함
	{
		std::promise<string> promise;
		std::future<string> future = promise.get_future();	// future data를 받아올 수 있게 연동된 상태

		// promise 소유권을 thread로 이전, 이후에 promise는 empty 상태
		thread t(PromiseWorker, std::move(promise));

		// future.get()은 단 한 번만 사용 가능. 이후 호출할 수 없음
		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		// int64(void) -> output은 int64, input은 void라는 의미 ( 예제 함수: int64 func(void) )
		std::packaged_task<int64(void)> task(future_Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// 결론은......
	// mutex, condition_variable까지 가지 않고 단순한 동작을 처리할 수 있다.
	// 특히, 한 번, 단발성으로 발생하는 이벤트에 유용.

	// 1) async - 원하는 함수를 비동기적으로 실행.

	// 2) promise - 결과물을 promise를 통해 future로 받아줌.

	// 3) packaged_task - 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌.

	return 0;
}