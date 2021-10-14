#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// STL �ڷᱸ�� �����̳ʵ��� Multi-Thread�� ȯ�濡���� �������� ������ �⺻������ ������.
// ex) 
// [1][2][3] 
// vector�� ����� ��á�ٸ�, ���� �޸� ������ ����� �� ū �޸𸮸� �Ҵ�����. (resize)
// �� �������� Multi-Thread ȯ���̶��, ������ ������ �޸� ������ �����Ϸ��ϱ� ������ ũ���ð� �߻���.

// ����� vector �������� ��쿣 [1][2][3][][][][][][][][] 
// ���ÿ� ������ ���, ũ���ô� ���� �ʰ����� ���� ��ġ�� �����͸� push�� ���� �־ �����Ͱ� �����Ǵ� ��찡 �߻���.

// STL �ڷᱸ���� ��� Atomic�� ����� �� ����

vector<int32> v;

// Mutual Exclusive ( ��ȣ��Ÿ�� )
// Lock : �� ���� �� Thread�� ���� �����ϵ��� �ϴ� ����
mutex lock_m;	// ������ �ڹ���. 


// RAII (Resource Acqusition Is Initialization)
// ���� Ŭ������ �����, �����ڿ��� lock���ְ� �Ҹ��ڿ� unlock����.
// mutex�� lock, unlock�� �ڵ�ȭ.
// ���� ���� ������ ǥ�ؿ� �߰��Ǿ� ����. ( std::lock_guard<T> )
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard()
	{
		_mutex->unlock();
	}

private:
	T* _mutex;
};

// lock�� ���ִ� ��ġ�� ����ؾ� ���� ���.
void Push()
{
	for (int32 i = 0; i < 10000; ++i)
	{
		// Thread�� ȣ��Ǹ� ���( �ڹ��� ��ױ� ) 
		// �ٸ� Thread�� �����ϴ� ���, unlock()�� ȣ��Ǳ� �������� ������ �� ����. 
		// ���������� single thread�� �����ϰ� �ϴ� ���̱� ������, �Ϲ����� Multi-thread���� ������ ����.

		//std::lock_guard<std::mutex> lockGuard(m);

		// lock_guard�� ������ ������ ������, ���� �ɼ��� �߰��� �� ����. (�������� ����)
		// std::defer_lock�� �Ѵٸ�, ���� �� ���°� lock�� ���� �ʰ� ����.
		std::unique_lock<std::mutex> uniqueLock(lock_m, std::defer_lock);
		uniqueLock.lock();	// �� ��, lock�� �ɸ�.

		//LockGuard<std::mutex> lockGuard(m);

		//m.lock();
		//m.lock();

		v.emplace_back(i);

		if (i == 4999)
		{
			//m.unlock();
			break;
		}

		// unlock�� ���� �ʴ´ٸ�, ������ �ٸ� thread���� ������ �� ����.
		//m.unlock();	// �ڹ��� Ǯ��

		//m.unlock()
		// mutex�� ��������� lock�� �� �� ����. (reculsive mutex�� ����ؾ� ��)
	}
}

int lock_main()
{
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;

	return 0;
}