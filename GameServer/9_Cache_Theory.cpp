#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>
#include <future>

//	==== 컴퓨터 구조 원리 : 캐시(Cache)와 파이프라인 ====
//	
//	CPU는 대단히 빠른 연산기능을 자랑하지만, RAM의 메모리에서 데이터를 가져오는 동작은 매우 무겁다.
//	
//	이런 문제를 해결하기 위해 캐시(Cache)가 등장.
//	
//	
//	Register->L1 Cache->L2 Cache .....
//	
//	사용 빈도와 중요도가 높을 수록 먼저 저장되고, 이 캐시 중에도 데이터가 없다면, 그 때 메인 메모리(RAM)에서 찾는다.
//	
//	최대한 메인 메모리까지 갈 필요가 없도록 하기 위한 중간 저장소로 생각하면 쉬울 듯.
//	
//	
//	
//	-캐시 철학
//	
//		1) Temporal Locality(시간적 위치)
//			시간적으로 보면, 방금 주문한 테이블에서 추가 주문이 나올 확률이 높다.
//			방금 주문할 걸 메모해 놓으면 편하지 않을까 ?
//		
//		2) Spatial Locality(공간적 위치)
//			공간적으로 보면, 방금 주문한 사람 근처에 있는 사람이 추가 주문을 할 확률이 높다.
//			방금 주문한 사람과 합석하고 있는 사람들의 주문 목록도 메모해 놓으면 편하지 않을까 ?



// 2차원 배열 방식
// (가시성을 위함이지, 실제로는 붙어있음)
// [][][][][]		[][][][][]		[][][][][]		.....
int32 cache_buffer[10000][10000];

int Cache_main()
{
	memset(cache_buffer, 0, sizeof(cache_buffer));

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10000; ++i)
		{
			for (int32 j = 0; j < 10000; ++j)
			{
				// [i][j] - 순차적 순회일 경우
				// 인접한 배열이 cache에 들어가 있을 확률이 높다.
				// 해당 위치의 배열의 데이터가 cache에 있음. (이런 경우를 cache hit라고 함)
				// 따라서 대부분의 연속된 데이터들이 cache에 있기 때문에, 동작이 빠름.
				sum += cache_buffer[i][j];
			}
		}

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
	}


	{
		uint64 start = GetTickCount64();

		int64 sum = 0;

		for (int32 i = 0; i < 10000; ++i)
		{
			for (int32 j = 0; j < 10000; ++j)
			{
				// [j][i] - 1차 인덱스가 변경되고 2차 인덱스는 고정되는 방식
				// j가 증가하는 것은 다음 블록으로 가는 것.
				// 크게 이동하며 데이터를 불러오기 때문에, cache에 원하는 데이터가 없을 확률도 높아짐. (cache hit가 될 확률이 떨어짐)
				// 따라서 위의 [i][j] 순회보다 느릴 수 밖에 없다.
				sum += cache_buffer[j][i];
			}
		}

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
	}

	return 0;
}