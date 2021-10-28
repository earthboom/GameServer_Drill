#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//	==== CPU 파이프라인 ====

// 비유)
// 세탁소에서 빨래를 하는 상황, 빨래감이 담긴 여러 바구니가 왔음.
// 1) 세탁기에 빨래를 한다.
// 2) 건조기에 넣어 건조.
// 3) 꺼내서 다리미질.
// 4) 옷을 정리.

// CPU - PipeL ine
// 1) Fetch : 명령어를 가져오고
// 2) Decode : 명령어를 해석
// 3) Execlute : 실행
// 4) Write-back : 결과를 다시 갖다줌.



// 가시성, 코드 재배치 문제 발생.

// 컴파일러 판단으로 효율이 있고, 같은 결과가 나올 경우에 코드가 재배치 되는 경우가 있음.
int32 cpu_x = 0;
int32 cpu_y = 0;
int32 cpu_r1 = 0;
int32 cpu_r2 = 0;

volatile bool cpu_ready;

void cpu_Thread_1()
{
	while (!cpu_ready)
		;

	cpu_y = 1;		// Store y
	cpu_r1 = cpu_x;	// Load x
}

void cpu_Thread_2()
{
	while (!cpu_ready)
		;

	cpu_x = 1;		// Store x
	cpu_r2 = cpu_y; // Load y
}

int CPU_PipeLine_main()
{
	int32 count = 0;

	while (true)
	{
		cpu_ready = false;

		++count;

		cpu_x = cpu_y = cpu_r1 = cpu_r2 = 0;

		thread t1(cpu_Thread_1);
		thread t2(cpu_Thread_2);

		cpu_ready = true;

		t1.join();
		t2.join();

		if (cpu_r1 == 0 && cpu_r2 == 0)
			break;
	}

	cout << count << " 번만에 빠져나왔다." << endl;

	return 0;
}