#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>	// Window, Linux ������� �������� Thread ������ ���� (C++ 11 ����)

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread_2(int32 num)
{
	cout << num << endl;
}

int thread_main()
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

	return 0;
}
