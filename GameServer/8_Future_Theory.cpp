#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//===== Future ======
// C++11 ���� �߰�.

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
	// ����(synchronous) ���� (�Ϲ���)
	// ȣ��� �Լ��� �ſ� ���ſ� ������ �ϸ�, main thread�� ���������� ������ �߻��� �� ����.
	// ���� �ٸ� thread�� ���ѱ�� �񵿱��� ������ �ʿ��� ���� ����.
	//int64 sum = future_Calculate();
	//cout << sum << endl;


	// �Ϲ������� thread�� �����, ���� ������ ���������� �̺��� �� ������ ����� ����.
	// ��ġ �ܱ� �˹ٿ� ���� ����?

	// std::futrue
	// ������ �̷��� ������� ����� ��!
	{
		// deferred -> lazy evaluation �����ؼ� �����϶�.(���� ��������� �ڷ� �̷�� ��.)
		// async -> ������ thread�� ����� ����.(multi-thread)
		// deferred | async -> �� �� �˾Ƽ� ����.
		std::future<int64> future = std::async(std::launch::async, future_Calculate);

		// TODO

		// std::future_status �� future ��ü�� �۾� ���¸� �� �� ����.
		//std::future_status status = future.wait_for(1ms);	//��� �Լ�
		//if (status == future_status::ready)	// ������� �غ�� ����.
		//{
		//}


		// ������� �ʿ��� ����
		// deferred �ɼ��� ��쿣, �� �������� �Լ��� ȣ���.
		// aync �ɼ��� ���, thread���� ���� ������ ����� �ҷ���. ��� ���� ���̶�� ���.
		int64 sum = future.get();

		// class�� ���� �ɹ��Լ��� ȣ���� ���
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
	// �̷�(std::future)�� ������� ��ȯ���� ���̶�� ���(std::promise) - ��༭�� ������
	{
		std::promise<string> promise;
		std::future<string> future = promise.get_future();	// future data�� �޾ƿ� �� �ְ� ������ ����

		// promise �������� thread�� ����, ���Ŀ� promise�� empty ����
		thread t(PromiseWorker, std::move(promise));

		// future.get()�� �� �� ���� ��� ����. ���� ȣ���� �� ����
		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		// int64(void) -> output�� int64, input�� void��� �ǹ� ( ���� �Լ�: int64 func(void) )
		std::packaged_task<int64(void)> task(future_Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;

		t.join();
	}

	// �����......
	// mutex, condition_variable���� ���� �ʰ� �ܼ��� ������ ó���� �� �ִ�.
	// Ư��, �� ��, �ܹ߼����� �߻��ϴ� �̺�Ʈ�� ����.

	// 1) async - ���ϴ� �Լ��� �񵿱������� ����.

	// 2) promise - ������� promise�� ���� future�� �޾���.

	// 3) packaged_task - ���ϴ� �Լ��� ���� ����� packaged_task�� ���� future�� �޾���.

	return 0;
}