#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//	==== CPU ���������� ====

// ����)
// ��Ź�ҿ��� ������ �ϴ� ��Ȳ, �������� ��� ���� �ٱ��ϰ� ����.
// 1) ��Ź�⿡ ������ �Ѵ�.
// 2) �����⿡ �־� ����.
// 3) ������ �ٸ�����.
// 4) ���� ����.

// CPU - PipeL ine
// 1) Fetch : ��ɾ ��������
// 2) Decode : ��ɾ �ؼ�
// 3) Execlute : ����
// 4) Write-back : ����� �ٽ� ������.



// ���ü�, �ڵ� ���ġ ���� �߻�.

// �����Ϸ� �Ǵ����� ȿ���� �ְ�, ���� ����� ���� ��쿡 �ڵ尡 ���ġ �Ǵ� ��찡 ����.
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

	cout << count << " ������ �������Դ�." << endl;

	return 0;
}