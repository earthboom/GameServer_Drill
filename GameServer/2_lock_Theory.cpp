#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

// STL 자료구조 컨테이너들은 Multi-Thread의 환경에서는 동작하지 않음을 기본적으로 가정함.
// ex) 
// [1][2][3] 
// vector의 사이즈가 다찼다면, 기존 메모리 영역을 지우고 더 큰 메모리를 할당해줌. (resize)
// 이 과정에서 Multi-Thread 환경이라면, 기존의 지워진 메모리 영역을 참조하려하기 때문에 크래시가 발생함.

// 충분한 vector 사이즈일 경우엔 [1][2][3][][][][][][][][] 
// 동시에 접근할 경우, 크래시는 나지 않겠지만 같은 위치에 데이터를 push할 수도 있어서 데이터가 누락되는 경우가 발생함.

// STL 자료구조의 경우 Atomic을 사용할 수 없음

vector<int32> v;

// Mutual Exclusive ( 상호배타적 )
// Lock : 한 번에 한 Thread만 접근 가능하도록 하는 개념
mutex lock_m;	// 일종의 자물쇠. 


// RAII (Resource Acqusition Is Initialization)
// 래퍼 클래스를 만들어, 생성자에서 lock해주고 소멸자에 unlock해줌.
// mutex의 lock, unlock를 자동화.
// 직접 만들어도 되지만 표준에 추가되어 있음. ( std::lock_guard<T> )
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

// lock을 해주는 위치도 고민해야 함을 명심.
void Push()
{
	for (int32 i = 0; i < 10000; ++i)
	{
		// Thread로 호출되면 잠김( 자물쇠 잠그기 ) 
		// 다른 Thread는 접근하는 경우, unlock()이 호출되기 전까지는 접근할 수 없음. 
		// 강제적으로 single thread로 동작하게 하는 것이기 때문에, 일반적인 Multi-thread보다 동작이 느림.

		//std::lock_guard<std::mutex> lockGuard(m);

		// lock_guard와 동일한 역할을 하지만, 좀더 옵션을 추가할 수 있음. (유연성이 있음)
		// std::defer_lock을 한다면, 선언만 한 상태고 lock을 하지 않고 있음.
		std::unique_lock<std::mutex> uniqueLock(lock_m, std::defer_lock);
		uniqueLock.lock();	// 이 때, lock이 걸림.

		//LockGuard<std::mutex> lockGuard(m);

		//m.lock();
		//m.lock();

		v.emplace_back(i);

		if (i == 4999)
		{
			//m.unlock();
			break;
		}

		// unlock을 하지 않는다면, 영원히 다른 thread들이 접근할 수 없음.
		//m.unlock();	// 자물쇠 풀기

		//m.unlock()
		// mutex는 재귀적으로 lock을 할 수 없음. (reculsive mutex를 사용해야 함)
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