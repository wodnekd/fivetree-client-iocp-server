#include "stdafx.h"
#include <string>


CLogin::CLogin()
{
}


CLogin::~CLogin()
{
}

//1.
BOOL CLogin::CompareUserID(LPWSTR LoginID)
{
	char tempID[10] = "";
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, LoginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, LoginID, -1, tempID, len, NULL, NULL);

	m_QueryState = mysql_query(Database.GetConnection(), "select * from login3");
	if (m_QueryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&Database.GetConn()));
		return FALSE;
	}
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));

	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		if (strcmp(Database.GetSqlRow()[0], tempID) == 0)
		{
			return TRUE;
		}
	}
	mysql_free_result(Database.GetSqlResult());
	return FALSE;
}

BOOL CLogin::CreateUserID(LPWSTR LoginID, LPWSTR Pass, LPWSTR Nickname, int CharacterNum)
{
	char tempID[20] = "";
	char tempPW[20] = "";
	char tempNickname[20] = "";
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, LoginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, LoginID, -1, tempID, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, Pass, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, Pass, -1, tempPW, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, Nickname, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, Nickname, -1, tempNickname, len, NULL, NULL);

	sprintf(m_Query, "insert into login3 values ('%s', '%s', '%s', '%d', '%d')", tempID, tempPW, tempNickname, CharacterNum, 0);
	m_QueryState = mysql_query(Database.GetConnection(), m_Query);
	if (m_QueryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s \n", mysql_error(&Database.GetConn()));
		return FALSE;
	}
	return TRUE;
}


BOOL CLogin::LoginUser(LPWSTR loginID, LPWSTR pass)
{
	char tempID[20] = "";
	char tempPW[20] = "";
	int len;
	BOOL m_idCheck;
	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	len = WideCharToMultiByte(CP_ACP, 0, pass, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, pass, -1, tempPW, len, NULL, NULL);

	m_QueryState = mysql_query(Database.GetConnection(), "select * from login3");
	if (m_QueryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&Database.GetConn()));
		return FALSE;
	}
	//로그인 테이블 컬럼을 한꺼번에 가져오기
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));

	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		if (strcmp(Database.GetSqlRow()[0], tempID) == 0)
		{
			m_idCheck = TRUE;
			if (strcmp(Database.GetSqlRow()[1], tempPW) == 0)
			{
				cout << "로그인 완료" << endl;
				return TRUE;
			}
			else
			{
				cout << "비밀번호 불일치 로그인 불가" << endl;
				return FALSE;
			}
		}
		else
		{
			m_idCheck = FALSE;
		}	
	}
	if (m_idCheck == FALSE)
	{
		cout << "아이디가 존재하지 않음 로그인 불가" << endl;
		return FALSE;
	}
	mysql_free_result(Database.GetSqlResult());
	return TRUE;
}

LPWSTR CLogin::GetNickname(LPWSTR loginID)
{
	char tempID[10] = "";
	char tempNickname[10] = "";
	WCHAR wtempNickname[10] = L"";
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	m_QueryState = mysql_query(Database.GetConnection(), "select * from login3");
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));

	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		if (strcmp(Database.GetSqlRow()[0], tempID) == 0)
		{
			//cout << Database.GetSqlRow()[2] << endl;
			strcpy(tempNickname, Database.GetSqlRow()[2]);
			len = MultiByteToWideChar(CP_ACP, 0, tempNickname, strlen(tempNickname), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, tempNickname, strlen(tempNickname), wtempNickname, len);
			return wtempNickname;
		}
	}
	return wtempNickname;
}

int CLogin::GetCharacterNum(LPWSTR loginID)
{
	char tempID[10] = "";
	int characterNum = 0;
	int len;

	len = WideCharToMultiByte(CP_ACP, 0, loginID, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, loginID, -1, tempID, len, NULL, NULL);

	m_QueryState = mysql_query(Database.GetConnection(), "select * from login3");
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));

	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		if (strcmp(Database.GetSqlRow()[0], tempID) == 0)
		{
			characterNum = stoi(Database.GetSqlRow()[3]);
			return characterNum;
		}
	}
}

BOOL CLogin::DeleteUserID()
{
	printf("삭제할 유저 아이디 입력 : ");
	fgets(m_Name, 20, stdin);
	CHOP(m_Name);

	sprintf(m_Query, "delete from login3 where id = '%s' ", m_Name);

	m_QueryState = mysql_query(Database.GetConnection(), m_Query);

	if (m_QueryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&Database.GetConn()));
		return FALSE;
	}
	mysql_free_result(Database.GetSqlResult());
	return TRUE;
}
