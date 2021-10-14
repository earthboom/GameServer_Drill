#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

// 이를 방지하기 위해서 mutex를 class로 래핑하여 id를 매기는 방법도 있음을 참고

// mutex를 추적하는 Manager를 생성하여, graph 알고리즘을 적용해 
// 1->2->3처럼 순서가 아닌 1->2->1 과 같은 사이클이 일어나면 데드락으로 추정하여 수정하는 방법

void AccountManager::ProcessLogin()
{
	// 동시에 Lock이 걸리는 순간.
	
	// Dead Lock 상태를 해결하기 위한 위치 변경.
	// 모든 경우에 이렇게 순서를 맞추는 것은 쉬운 일이 아님.
	User* user = UserManager::instance()->GetUser(100);

	// account Lock
	lock_guard<mutex> guard(_mutex);

	// user Lock ( Dead Lock 이 걸리는 위치 )
	// 다른 Thread가 이미 UserManager의 ProcessSave를 호출하여 
	// UserManager의 mutex가 Lock을 걸었기 때문에, 접근할 수가 없어 무한정 대기 상태.
	// 이는 UserManager의 ProcessSave를 실행중인 다른 Thread도 마찬가지다. (교착상태)
	//User* user = UserManager::instance()->GetUser(100);

	// TODO
}
