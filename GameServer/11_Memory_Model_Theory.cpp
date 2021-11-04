#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//	==== �޸� �� ====
// C++ 11 ���� �߰��� Momory Model

// atomic ���꿡 ���ؼ�, ��� Thread�� ���� ��ü�� ���ؼ� ������ ���� ������ ���� (�������� ��Ģ)
// atomic(������) ������ CPU�� �� ���� ó���� �� �ִ� ������ �ǹ�.

//atomic<int64> num;
// ��� ���� �ǹ�.
//num = 1;
//num.store(1);
//num.store(1, memory_order::memory_order_seq_cst);

atomic<bool> flag = false;

int Atomic_Func()
{
	// lock�� ���� �ǹ�
	// true  : �ش� �����͸� ���������� ó���� �� �ֱ� ������ lock�� �� �ʿ� ���� ������ ó���� �����ϴٴ� �ǹ�.
	// false : �ش� �����͸� ���������� ó���� �� ���� ������(CPU), lock�� �ɾ� �����͸� ó���ؾ� �Ѵٴ� �ǹ�.  
	//flag.is_lock_free();	

	//flag = true;	// <- �̷��� �ᵵ �ǳ�, atomic �������� ǥ���ϱ� ���� �Ʒ��� ���� ���� �� ��õ.
	flag.store(true, memory_order::memory_order_seq_cst);

	//bool val = flag; // <- ���� ���� ����
	bool val = flag.load(memory_order::memory_order_seq_cst);

	// ���� flag ���� prev�� �ְ�, flag���� ����
	{
		//bool prev = flag;	
		//flag = true;
		// ���� ���� �߰��� �ٸ� thread���� �����͸� ������ ���ɼ��� �ֱ� ������
		// ���� ����� ���� ������ �� ���� ��.
		bool prev = flag.exchange(true);
	}

	// CAS (Compare-And-Swap) ���Ǻ� ����
	{
		bool expected = false;	// ���� ��
		bool desired = true;	// ���ϴ� ��
		flag.compare_exchange_strong(expected, desired);

		// compare_exchange_strong �Լ� ������ ǥ���� �ڵ�
		//if (flag == expected)	// flag���� ����(expected)�� ���ٸ�
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
		// ���� compare_exchange_strong �� �Ȱ��� ������.
		// �׷��� �߰��� �ٸ� Threadd�� interruption�� �޾� �߰��� ������ ���� �ִ� ��Ȳ�� �����Ͽ�
		// �׷� ��Ȳ������ ���и� ����ϰ� ����.
		// �̸� ��¥ ����(Spurious Failure)�̶� ��.
		// ��ǻ� �̰� �⺻���� �Լ��̰� strong�� ���ϴ� ����� ���� ������ �ݺ��ϴ� �Լ�.
		// ���� weak���� strong�� �� �� ���ſ� �Լ�. 
		// �׷��� weak�� ����ϸ� while���� ���� ���. (strong�� ũ�� ���� ������ ����.)
	}

	return 0;
}


atomic<bool> mm_ready;
int32 mm_value;

void mm_Producer()
{
	mm_value = 10;

	mm_ready.store(true, memory_order::memory_order_release);
	// memory_order_release ����̸�, ���� �ڵ尡 �ش� ���� �Ʒ��� ���ġ �� �� ���� ��.
	// memory_order_release ���� require�� ¦�� ����� ��.
	// atomic �Ӹ��� �ƴ϶� std::atomic_thread_fence ���� ���ؼ� �̷� ������ ������.
}

void mm_Consumer()
{
	// memory_order_acquire ����, �Ʒ��� �ڵ尡 �ش� ���� ���� ���ġ �� �� ���� ��.
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


	// Memory Model (��å)
	// 1) Sequentially Consistent ( seq_cst )
	// 2) Acquire-Release ( acquire, release)   �� �� ������ �ִٰ� �˾Ƶֶ� -> (consume, acq_rel)
	// 3) Relaxed ( relaxed )

	// 1) seq_cst			(���� ���� = �����Ϸ� ����ȭ ������ ���� = ������)
	// ���ü� ���� �ٷ� �ذ�! �ڵ� ���ġ ���� �ذ�!
	// ������ ���� ��κ�! �����ϸ�! �̴�� ����� (default)

	// 2) acquire_relase
	// �� �߰� (����������, ���������� ����)
	// release ��� ������ �޸� ��ɵ���, �ش� ��� ���ķ� ���ġ �Ǵ� ���� ����!
	// �׸��� acquire�� ���� ������ �д� thread�� �ִٸ�
	// release ������ ��ɵ��� acquire �ϴ� ������ ������ ����! (���ü� ����)

	// 3) relaxed			(�����Ӵ� = �����Ϸ� ����ȭ ���� ���� = ���������� ����)
	// �ʹ����� �����ο�!
	// �ڵ� ���ġ�� �ڴ�� ����! ���ü� �ذ� �ȵ�!
	// ���� �⺻ ���� (���� ��ü�� ���� ���� ���� ������ ����)
	// ���� ���� ������ ���� Ȱ������ ����.


	// Intel, AMD �� ��� �ִ��� �������� �ϰ����� ����!
	// seq_cst�� �ᵵ ���ٸ� ���̰� ����. (�����ϸ� �״�� ���ٴ� �ǹ�)

	// ARM ��쿣 �ϳ� ���̰� �ִٰ� ��.

	return 0;
}