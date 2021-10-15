#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

//===== Spin Lock ======
// ���տ� ����, ��� ���� ��, ������ �����·� Lock�� ��û�ϰ� �ִ� ���¶�� �����ϸ� ��.

// while���� ���� User level������ �����ϱ� ������, Context Switching�� ��¡ �ʾ� ������ ����.



// ���� Thread�� �޸𸮿� �����ϰ� �ڹ���(Lock)�� ����ٰ� ���ÿ� �����ϴ� ��찡 ����.
// 
// Thread�� �޸𸮸� �����ϰ� �ڹ���(Lock)�� ��� ������ �и��Ǿ �ȵǰ�,
// �׷��� �ȵɰŸ�, �ƿ� ���������� �� �� �����ؾ� �� (Atomic�� ����)

class SpinLock
{
public:
	void lock()
	{
		// CAS (Compare-And-Swap)

		bool expected = false;	// ���� ���� ��Ȳ (Before)
		bool desired = true;	// �ٲ�� ���ϴ� ������ ��Ȳ (After)

		// CAS �ǻ��ڵ�
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

		// atomic �Լ��� compare_exchange_strong�� ���� CAS �ǻ��ڵ带 �� ���� ������. (atomic�� ����)
		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			// ���� ������ ���鼭 ��� �õ��ϴ� ��.
			// �ش� Thread�� ���� ���� ���� �ð��� ��� ����. (Spin Lock Ư¡)
			// expected �� true�� �ɶ����� �̰� Spin Lock
			expected = false;
		}




		//while (_locked)
		//{
		//}
		////���ÿ� �����Ͽ�, while���� �׳� �پ�Ѵ� ��찡 ���� �� ����.
		//_locked = true;
	}

	void unlock()
	{
		//_locked = false;

		_locked.store(false);
	}

private:
	// volatile  ( C++ )
	// �����Ͽ��� ����ȭ�� ���� ����� Ű����.
	//volatile bool _locked = false;

	// atomic�� volatile Ű���嵵 �����ϰ� ����.
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
	// volatile Ű����
	// 
	// Release ����� ���.
	// �������� �ڵ����� ����ȭ�Ͽ� int32 a = 4 �� ����.
	// ���⿡ volatile Ű���带 ������, ����ȭ�� �������� ����.
	//volatile int32 a = 0;
	//a = 1;
	//a = 2;
	//a = 3;
	//a = 4;
	//cout << a << endl;

	// �⺻������ flag�� bool üũ�� �ǹ̰� ���ٰ� �Ǵ���.
	// ������ Ȯ���ϵ��� �Ϸ��� volatile Ű���带 �߰�
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