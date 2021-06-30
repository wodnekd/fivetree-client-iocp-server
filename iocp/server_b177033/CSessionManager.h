#pragma once

class CSessionManager : public CMultiThreadSync <CSessionManager>
{
private:
	CUserManager *userManager;
	CRoomManager *RoomManager;
	std::list<CSession*>					InActiveSessionList;
	std::map<SOCKET, CSession*>				ActiveSessionListMap;

public:
	CSessionManager();
	~CSessionManager();
	VOID Init();
	BOOL CreateSeccsion(int SessionNumber);
	//��ü Ǯ�� Ȱ��ȭ ��Ȱ��ȭ
	VOID ActiveSession(CSession *pSession);
	VOID InActiveSession(CSession *pSession);
	VOID Clear();
	CUserManager* GetUserManager()  { return userManager; }
	CRoomManager* GetRoomManager()  { return RoomManager; }
};

