#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//	==== 메모리 모델 ====
// C++ 11 에서 추가된 Momory Model

// atomic 연산에 한해서, 모든 Thread가 동일 객체에 대해서 동일한 수정 순서를 관찰 (절대적인 법칙)
// atomic(원자적) 연산은 CPU가 한 번에 처리할 수 있는 연산을 의미.

//atomic<int64> num;
// 모두 같은 의미.
//num = 1;
//num.store(1);
//num.store(1, memory_order::memory_order_seq_cst);

atomic<bool> flag = false;

int Atomic_Func()
{
	// lock이 없다 의미
	// true  : 해당 데이터를 원자적으로 처리할 수 있기 때문에 lock을 걸 필요 없이 데이터 처리가 가능하다는 의미.
	// false : 해당 데이터를 원자적으로 처리할 수 없기 때문에(CPU), lock을 걸어 데이터를 처리해야 한다는 의미.  
	//flag.is_lock_free();	

	//flag = true;	// <- 이렇게 써도 되나, atomic 인자임을 표현하기 위해 아래와 같이 쓰는 걸 추천.
	flag.store(true, memory_order::memory_order_seq_cst);

	//bool val = flag; // <- 위와 같은 이유
	bool val = flag.load(memory_order::memory_order_seq_cst);

	// 이전 flag 값을 prev에 넣고, flag값을 수정
	{
		//bool prev = flag;	
		//flag = true;
		// 위의 과정 중간에 다른 thread에서 데이터를 변경할 가능성이 있기 때문에
		// 밑의 방식은 위의 과정을 한 번에 함.
		bool prev = flag.exchange(true);
	}

	// CAS (Compare-And-Swap) 조건부 수정
	{
		bool expected = false;	// 예상 값
		bool desired = true;	// 원하는 값
		flag.compare_exchange_strong(expected, desired);

		// compare_exchange_strong 함수 과정을 표현한 코드
		//if (flag == expected)	// flag값이 예상값(expected)와 같다면
		//{
		//	//expected = flag
		//	flag = desired;
		//	return true;
		//}
		//else
		//{
		//	flag = expected;
		//	return false;
		//}


		// flag.compare_exchange_weak(expected, desired);
		// 위의 compare_exchange_strong 와 똑같이 동작함.
		// 그러나 중간에 다른 Threadd의 interruption을 받아 중간에 실패할 수도 있는 상황을 포함하여
		// 그런 상황에서의 실패를 고려하고 있음.
		// 이를 가짜 실패(Spurious Failure)이라 함.
		// 사실상 이게 기본적인 함수이고 strong은 원하는 결과가 나올 때까지 반복하는 함수.
		// 따라서 weak보다 strong이 좀 더 무거운 함수. 
		// 그래서 weak를 사용하면 while문과 같이 사용. (strong과 크게 차이 나지는 않음.)
	}

	return 0;
}


atomic<bool> mm_ready;
int32 mm_value;

void mm_Producer()
{
	mm_value = 10;

	mm_ready.store(true, memory_order::memory_order_release);
	// memory_order_release 모드이면, 위의 코드가 해당 라인 아래로 재배치 될 수 없게 함.
	// memory_order_release 모드면 require와 짝을 맞춰야 함.
	// atomic 뿐만이 아니라 std::atomic_thread_fence 등을 통해서 이런 통제가 가능함.
}

void mm_Consumer()
{
	// memory_order_acquire 모드는, 아래의 코드가 해당 라인 위로 재배치 될 수 없게 함.
	while (mm_ready.load(memory_order::memory_order_acquire) == false)
		;

	cout << mm_value << endl;
}

int mm_main()
{
	mm_ready = false;
	mm_value = 0;

	thread t1(mm_Producer);
	thread t2(mm_Consumer);

	t1.join();
	t2.join();


	// Memory Model (정책)
	// 1) Sequentially Consistent ( seq_cst )
	// 2) Acquire-Release ( acquire, release)   잘 안 쓰여서 있다고만 알아둬라 -> (consume, acq_rel)
	// 3) Relaxed ( relaxed )

	// 1) seq_cst			(가장 엄격 = 컴파일러 최적화 여지가 적음 = 직관적)
	// 가시성 문제 바로 해결! 코드 재배치 문제 해결!
	// 때문에 거의 대부분! 웬만하면! 이대로 사용함 (default)

	// 2) acquire_relase
	// 딱 중간 (엄격하지도, 자유롭지도 않은)
	// release 명령 이전의 메모리 명령들이, 해당 명령 이후로 재배치 되는 것을 금지!
	// 그리고 acquire로 같은 변수를 읽는 thread가 있다면
	// release 이전의 명령들이 acquire 하는 순간에 관찰이 가능! (가시성 보장)

	// 3) relaxed			(자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 너무나도 자유로움!
	// 코드 재배치도 멋대로 가능! 가시성 해결 안됨!
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)
	// 위의 이유 때문에 거의 활용하지 않음.


	// Intel, AMD 의 경우 애당초 순차적인 일관성을 보장!
	// seq_cst를 써도 별다른 차이가 없음. (웬만하면 그대로 쓴다는 의미)

	// ARM 경우엔 꽤나 차이가 있다고 함.

	return 0;
}