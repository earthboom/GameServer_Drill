#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>	// Window, Linux ������� �������� Thread ������ ���� (C++ 11 ����)
#include <atomic>	// atomic : All-Or-Nothing. (�� ���� ���� ����ǰų� ���� �ʰų�) (Window, Linux ���� ��� ����)


int main()
{
	return 0;
}


//=========================
//===== Create Atomic =====
//=========================
/*
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

int main()
{
	Add();
	Sub();
	cout << sum << endl;

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;
}
*/

//=========================
//===== Create Thread =====
//=========================
/*
void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
	cout << num << endl;
}

int main()
{
	//HelloThread(); //MainThread�� ����

	vector<std::thread> v;

	// �ѹ��� ���� Thread�� ����� ���, ���� ���ķ� �����.
	// ���� ������ �۾��� �ѿ��� � Thread�� ������ ������ �������� ������ �� ����.
	for (int32 i = 0; i < 10; ++i)
	{
		v.emplace_back(std::thread(HelloThread_2, i));	// ȣ���� �Լ��� �Ű������� ���� �ÿ�.
	}

	for (int32 i = 0; i < 10; ++i)
	{
		if (v[i].joinable())
			v[i].join();
	}


	// Thread ����
	// - OS �� ��û.
	//std::thread t;	// thread�� ���������� ����.

	//auto id1 = t.get_id(); // �������� �ʱ� ������ 0 ��ȯ

	//t = std::thread(HelloThread); // ������ Thread�� ȣ��

	//int32 count = t.hardware_concurrency();	// CPU �ھ� ����? ���ÿ� ����� �� �ִ� �ھ� ������ �ǹ�. (��Ȯ������ �ʴٰ� ��.)

	//auto id2 = t.get_id();	// Thread������ ID

	// ������� Thread ��ü t�� ���������� ������ Thread���� ������� �����ִ� �ǹ�.
	// BackGround Thread�� ���������� �����̴� Thread�� �Ǵ� ��.
	// �� �̻� �ش� thread�� ���� ������ ������ �� ������.
	// �����ϸ� �� ������� ����.
	//t.detach();	// std::thread ��ü���� ���� Thread�� �и�

	// ���������� ������ Thread�� ���ٴ� ���� �Ǻ��ϱ� ���� �Լ�.
	// ���� thread�� �����ϴ°� ���ϴ°��� �Ǻ��ϱ� ����.
	//if (t.joinable())
	//{
	//	t.join();	// Thread t�� ����� ������ main thread�� ����ϵ��� �� ( Join() )
	//}

	cout << "Hello Main" << endl;
	// ������ thread�� ���� ���ε�, main Thread�� ���� ����Ǹ� ���� �߻��ϱ� ������ �������� ��.
}
*/
