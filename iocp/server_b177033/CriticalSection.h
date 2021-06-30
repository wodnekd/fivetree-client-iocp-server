#pragma once
#include<winsock2.h>

class CCriticalSection
{
private:
	CRITICAL_SECTION	m_Sync;

public:
	CCriticalSection()
	{
		InitializeCriticalSection(&m_Sync);
	}

	~CCriticalSection()
	{
		DeleteCriticalSection(&m_Sync);
	}

	inline void Enter()
	{
		EnterCriticalSection(&m_Sync);
	}

	inline void Leave()
	{
		LeaveCriticalSection(&m_Sync);
	}

};

//Enter 과 Leave의 짝이 맞지 않는 경우 데드락 발생
//이를 방지하기 위하여 생성자 소멸자를 통해 Enter과 Leave 관리
template <class T>
class CMultiThreadSync
{
friend class CThreadSync;					//CMutiThreadSync 의 private에 CThreadSync가 접근 가능

private:
	static CCriticalSection mSync;

public:
	class CThreadSync
	{
	public:
		CThreadSync()
		{
			T::mSync.Enter();
		}

		~CThreadSync()
		{
			T::mSync.Leave();
		}
	};

};

//전역 변수 선언으로 CCriticalSection를 생성하지 않고 사용
template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;