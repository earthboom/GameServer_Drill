#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

//===== Event ======
// Kernel �����ڿ��� ���ϴ� �޸��� lock�� Ǯ������ �˷��޶�� �ϴ� �� (Event)
// event�� ���ؼ�, ��� Thread�� �ƿ�� ����̶� �ſ� ȿ����
// Kernel mode���� Event�� �����ϴ� ���ſ� �۾��̶� �����ؼ� �ȵȴ�.

// Auto Reset Event
// - ���� �ڵ����� ���

//Manual Reset Event
// - ���� �������� ���

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

		// �ش� �ڵ��� ���� Kernel Object�� ���¸� Signal(�Ķ���)���·� �����ش޶�� ��û (true)
		::SetEvent(event_handle);

		this_thread::sleep_for(10000ms);	// 100ms = 0.1��
	}
}

void event_Consumer()
{
	// event_q�� �����͸� �־��ִ� �ð��� ��û���� �������, �� �Լ��� ��κ��� �۾��� �׿��۾��� �Ǿ� ����.
	// �ᱹ �̷��� �ݺ����� �۾��� �������� CPU�� �����ϰ� �Ǿ� ����.
	// ����, event_q�� �����͸� �־��� ���� �����ϵ��� �ϴ� Event�� ����� �ش�.

	while (true)
	{
		// �ش� �ڵ��� ���� Kernel Object�� ���°� Signal(�Ķ���)���°� �ɋ����� ���.
		// Non-Signal(������)�̸� �ι�° �Ű������� ����ŭ �ش� ��ġ���� thread�� �����
		::WaitForSingleObject(event_handle, INFINITE);

		// �ش� �ڵ��� Kernel Object�� bManualReset�� FALSE�̸� Auto Reset�̱� ������ 
		// �ڵ������� Non-Signal ���·� ���� �ȴ�.

		// TRUE�̸� Manual Reset�̾, �߰������� ������ Non-Signal ���·� ����� ��� �Ѵ�.
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

// Event�� Ȱ���ϸ� �ſ� ȿ�����̳�, Kernel level������ �۾��� �߰��Ǿ� ���̴�.
// ����, ���� �Ͼ�� �۾��̸� �𸣳�, �ſ� ����ϰ� �Ͼ�� �۾��� �̿��ϸ�, ������ ���α׷��� ���̰� �� �� �ִ�.
// Spin Lock, Sleep �� User level���� �����ϱ� ������ ���� ������ ������, ����� �۾��� ������ ������ �� �ִ�.

int Event_main()
{
	// HANDLE : ������ Event�� �����ϱ� ���� ������

	//Window API
	// Kernel���� �������, �׷��� "Kernel Object"��
	// Useage Count : �ش� Kernel Object�� ����� ����ϰ� �ִ°�?
	// Signal(�Ķ���, true) / Non-Signal(������, false) : Kernel Object�� ���� �� ���� ���� (boolean)
	// Auto / Manual : boolean
	event_handle = ::CreateEvent(
		NULL/*���ȼӼ�*/,
		FALSE/*bManualReset(TRUE : Manual Reset | FALSE : Auto Reset)*/,
		FALSE/*bInitialState(Event�� �ʱ� ����)*/,
		NULL/*Event �̸�*/
	);

	thread t1(event_Producer);
	thread t2(event_Consumer);

	t1.join();
	t2.join();

	::CloseHandle(event_handle);

	return 0;
}