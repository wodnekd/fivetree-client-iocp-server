#pragma once
class CLogin
{
private:
	char		 m_Name[20];
	char		 m_Password[20];
	char		 m_Query[255];
	int			 m_QueryState;

public:
	CLogin();
	~CLogin();
	BOOL CompareUserID(LPWSTR LoginID);
	BOOL CreateUserID(LPWSTR LoginID, LPWSTR Pass, LPWSTR Nickname, int CharacterNum);
	BOOL LoginUser(LPWSTR loginID, LPWSTR pass);
	LPWSTR GetNickname(LPWSTR loginID);
	int GetCharacterNum(LPWSTR loginID);
	BOOL DeleteUserID  ();
};

