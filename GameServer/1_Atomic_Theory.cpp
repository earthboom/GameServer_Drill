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
atomic<int32> sum = 0;

// Add(), Sub()�� ������ thread�� ���ÿ� ����� ���.
// ���� ������ sum�� ���� ���� ��������, ���� ������ �� ���� �Ǿ����.
// �̸� �ذ��ϱ� ���� ���� ����ȭ
void Add()
{
	//�Լ� ���� ����� ������ Stack ������ �ö�.
	//�ٸ� thread�� ������ ���� ����.
	for (int32 i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(1);
		//sum++;

		// sum++ ������ �ܰ躰�� ǥ��(eax�� CPU�� ��������)
		//int32 eax = sum;
		//eax = eax + 1;
		//sum = eax;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(-1);
		//sum--;

		// sum-- ������ �ܰ躰�� ǥ��(eax�� CPU�� ��������)
		//int32 eax = sum;
		//eax = eax - 1;
		//sum = eax;
	}
}

int atomic_main()
{
	Add();
	Sub();

	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();

	cout << sum << endl;

	return 0;
}
