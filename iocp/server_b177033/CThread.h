#pragma once

class CThread
{
private:
	SYSTEM_INFO			m_Si;
	HANDLE				m_HandleThread;
	vector<HANDLE>		ThreadListVector;
public:
	BOOL CreateThread();
	BOOL DestroyThread();
	static unsigned int WINAPI WorkerThread(LPVOID arg);
};

