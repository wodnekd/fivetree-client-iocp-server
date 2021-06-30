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

//Enter �� Leave�� ¦�� ���� �ʴ� ��� ����� �߻�
//�̸� �����ϱ� ���Ͽ� ������ �Ҹ��ڸ� ���� Enter�� Leave ����
template <class T>
class CMultiThreadSync
{
friend class CThreadSync;					//CMutiThreadSync �� private�� CThreadSync�� ���� ����

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

//���� ���� �������� CCriticalSection�� �������� �ʰ� ���
template <class T>
CCriticalSection CMultiThreadSync<T>::mSync;