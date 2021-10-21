#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

//===== Condition Variable (���� ����) ======
// C++ 11 �� ǥ��
// Linux, Window ���� ��� ����
// Event���� condition_variable ����� ��õ

mutex cv_m;
queue<int32> cv_q;
HANDLE cv_handle;

// mutex header�� ����.
// ǥ�� mutex ¦���� ����
// ���� - condition_variable�� User-Level Object(Ŀ�� ������Ʈ�� �ƴ�)
// cv�� Event�� �ſ� ����.
// (Ŀ�� ���� ������Ʈ�� �ٸ� ���α׷������� ���� �̺�Ʈ�� �̿��� ����ȭ�Ͽ� ����� ����.)
// (���� ���� ������Ʈ�� ������ ���α׷� ���ο����� ����� ����.)
condition_variable cv;

// ���� �Ϲ����� ��Ȳ���� Ȯ���ϰ� �ʹٸ�
//#include <condition_variable>
//condition_variable_any cv_any;

void cv_Producer()
{
	while (true)
	{
		// 1) Lock�� ���
		// 2) ���� �������� ����
		// 3) Lock�� Ǯ��
		// 4) ���� ����(CV)�� ���� �ٸ� Thread���� ����

		{
			unique_lock<mutex> lock(cv_m);	// Lock�� ���(1)
			cv_q.emplace(100);				// ���� �������� ����(2)
		}									// ������ ����� Lock�� Ǯ����(3)

		// ���� ����(CV)�� ���� �ٸ� Trhead���� ����(4)
		cv.notify_one();					// wait���� thread�� ������ �� 1���� �����.

		//cv.notify_all();					// ��� Thread���� ����

		//this_thread::sleep_for(10000ms);	// 100ms = 0.1��
	}
}

void cv_Consumer()
{
	while (true)
	{
		// 1) Lock�� ���
		// 2) ���� Ȯ��
		// - ���� O -> �������� �̾ �ڵ� ����
		// - ���� X -> Lock�� Ǯ���ְ� ��� ���·� ��ȯ

		unique_lock<mutex> lock(cv_m); // Lock�� ��� (1)

		// ���� Ȯ�� (2)
		// cv.wait�� ù��° �Ű������� Ÿ���� unique_lock.
		// CV���� ������ ������ ���, �߰��� Lock�� �����ؾ� �ϱ� ������, ������ unique_lock�� ���.
		// cv.wait�� �ι�° �Ű������� �Լ� ����Ʈ or ���ٷ� ��� ������ ������.
		cv.wait(lock, []() {return cv_q.empty() == false; });


		// �׷��� notify_one�� ������ �׻� ���ǽ��� �����ϴ� ���� �ƴұ�?
		// �̷� ��Ȳ�� Spurious Wakeup(��¥ ���?)
		// notify_one�� �� ��, Lock�� �׻� ��� �ִ� ���� �ƴϱ� ����.


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