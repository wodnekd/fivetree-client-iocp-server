#pragma once

class CUserManager : public CMultiThreadSync <CUserManager>
{
private:
	int clientNum;
	map<SOCKET,  CUserSession*>  userMapList;
	map<SOCKET,  CUserSession*>::iterator iter;

public:
	CUserManager();
	~CUserManager();
	BOOL Enter_User(CUserSession* user);
	BOOL Leave_User(CUserSession* leave);


	VOID Clear();
	// �� ��������Ʈ ����� ���� �Լ�
	std::map<SOCKET,  CUserSession*>  GetOnList()  {return userMapList;}
	int GetClientNum() { return clientNum; }
};

