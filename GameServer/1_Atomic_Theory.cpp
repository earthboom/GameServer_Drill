#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>	// Window, Linux ������� �������� Thread ������ ���� (C++ 11 ����)
#include <atomic>	// atomic : All-Or-Nothing. (�� ���� ���� ����ǰų� ���� �ʰų�) (Window, Linux ���� ��� ����)

//int32 sum = 0;	// ���� ������ Data ������ �ö�, ���� ��� thread�� ������ �� �ִ� ������

// �� Atomic ������ �ΰ� Thread ���� ������ ���̸�
// �ش� ������ ������ �۾����� Thread�� ������, �ٸ� Thread���� ª�� ����ϰ� ��.
// �� ��⸦ �ϰ� �ϴ� ���� CPU
// ���� : atomic ������� ����� �������� ���ſ�. ���������� ����� ����.
atomic<int32> atomic_sum = 0;

// Add(), Sub()�� ������ thread�� ���ÿ� ����� ���.
// ���� ������ sum�� ���� ���� ��������, ���� ������ �� ���� �Ǿ����.
// �̸� �ذ��ϱ� ���� ���� ����ȭ
void atomic_Add()
{
	// �Լ� ���� ����� ������ Stack ������ �ö�.
	// �ٸ� thread�� ������ ���� ����.
	for (int32 i = 0; i < 100'0000; ++i)
	{
		atomic_sum.fetch_add(1);
		//sum++;

		// sum++ ������ �ܰ躰�� ǥ��(eax�� CPU�� ��������)
		//int32 eax = sum;
		//eax = eax + 1;
		//sum = eax;
	}
}

void atomic_Sub()
{
	for (int32 i = 0; i < 100'0000; ++i)
	{
		atomic_sum.fetch_add(-1);
		//sum--;

		// sum-- ������ �ܰ躰�� ǥ��(eax�� CPU�� ��������)
		//int32 eax = sum;
		//eax = eax - 1;
		//sum = eax;
	}
}

int atomic_main()
{
	atomic_Add();
	atomic_Sub();

	cout << atomic_sum << endl;

	std::thread t1(atomic_Add);
	std::thread t2(atomic_Sub);
	t1.join();
	t2.join();

	cout << atomic_sum << endl;

	return 0;
}
