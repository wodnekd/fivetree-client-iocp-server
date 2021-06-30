#include "stdafx.h"

//DB define
//mysql 사용에 필요한 헤더


CDatabase::CDatabase()
{
}

CDatabase::~CDatabase()
{
	//데베 연결 해제
	mysql_close(m_Connection);
}

//mysql 연결
BOOL CDatabase::ConnectSql()
{
	//mysql 정보 초기화
	mysql_init(&m_Conn);
	//데베 연결
	m_Connection = mysql_real_connect(&m_Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306,(char*)NULL, 0);
	if (m_Connection == NULL)
	{
		fprintf(stderr, "Mysql connection error %d : %s \n",mysql_errno(&m_Conn), mysql_error(&m_Conn));
		return FALSE;
	}
	//데베 한글 안꺠짐
	mysql_query(m_Connection, "set names euckr");
	return TRUE;
}

MYSQL_RES* CDatabase::SetSqlResult(MYSQL_RES * mysql_store_result)
{
	m_SqlResult = mysql_store_result; 
	return m_SqlResult;
}

MYSQL_ROW CDatabase::SetSqlRow(MYSQL_ROW mysql_fetch_row)
{
	m_SqlRow = mysql_fetch_row;
	return m_SqlRow;
}
