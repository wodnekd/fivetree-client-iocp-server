#include "stdafx.h"



CRoom::CRoom(bool createRoom, bool full):m_createRoom(createRoom), m_full(full)
{
	user_num = 0;
	game_state = false;
	check = false;
}

CRoom::~CRoom()
{

}
//�뿡 ������ ����
bool CRoom::JoinUser(CUserSession* user) 
{
	CThreadSync Sync;
	if (IsRoomUser(user)) 
	{
		cout << "Current Is User" << endl;
		return false;
	}
	if (game_state)
	{
		return false;
	}
	if (IsFuLL())
	{
		return false;
	}
	useMapList.insert(std::map<SOCKET, CUserSession*>::value_type(user->GetSessionSock(), user));
	Update(user, NULL, JOIN, TRUE);

	return true;
}
//�뿡 ������ ����
bool CRoom::LeaveUser(CUserSession* user, void* pPacket, DWORD size) 
{
	CThreadSync Sync;
	if (!IsRoomUser(user)) 
	{
		return false;
	}
	else 
	{
		Broadcast_User(user, pPacket, size, false);
		iterMap = useMapList.find(user->GetSessionSock());
		if (iterMap != useMapList.end()) 
		{
			iterMap = useMapList.erase(iterMap);
			Update(user, NULL, LEAVE, TRUE);
		}
		//CSession *Session = dynamic_cast<CSession*>(user);
		//if (Session == NULL)
		//{
		//	cout << "dynamic_cast LeaveUser Faill" << endl;
		//}
		////SESSION.GetUser()->Leave_User(user);
	}

	return true;
}
//�뿡 ������ ����
bool CRoom::LeaveUser(CUserSession* user)
{
	CThreadSync Sync;
	if (!IsRoomUser(user))
	{
		return false;
	}
	else
	{
		iterMap = useMapList.find(user->GetSessionSock());
		if (iterMap != useMapList.end())
		{
			iterMap = useMapList.erase(iterMap);
			Update(user, NULL, LEAVE, TRUE);
		}
		CSession *Session = dynamic_cast<CSession*>(user);
		if (Session == NULL)
		{
			cout << "dynamic_cast LeaveUser Faill" << endl;
		}
		//SESSION.GetUser()->Leave_User(user);

	}
	return true;
}

//�濡 �ִ� �������� ��ü ����
bool CRoom::Broadcast_User(CUserSession *user, void* pPacket, DWORD size, bool all_user)
{
	CThreadSync Sync;
	if (IsRoomUser(user)) 
	{
		if (all_user)
		{
			for (iterMap = useMapList.begin(); iterMap != useMapList.end(); iterMap++)
			{
				if (IsRoomUser(iterMap->second))
					//(char*)
					iterMap->second->Send((char*)pPacket, size);
				cout << "��ü ���� �� " << endl;
			}
		}
		else
		{
			for (iterMap = useMapList.begin(); iterMap != useMapList.end(); iterMap++) 
			{
				if (iterMap->second == user) continue;
				//(char*)
				iterMap->second->Send((char*)pPacket, size);
				cout << "��ü ���� �� " << endl;
			}
		}
	}

	else
	{

		return false;
	}

	return true;
}

//bool CRoom::SendOhterUser(CUserSession * user)
//{
//	CThreadSync Sync;
//	if (IsRoomUser(user))
//	{
//		for (iterMap = useMapList.begin(); iterMap != useMapList.end(); iterMap++)
//		{
//			if (iterMap->second == user)
//			{
//				continue;
//			}
//			PAKCET_ENTER_FIVETREE_GAME_ACK send_packet;
//			wcscpy(send_packet.nickname, iterMap->second->nickname);
//			send_packet.characterNum = iterMap->second->characterNum;
//			send_packet.size = sizeof(send_packet);
//			send_packet.homeAway = HOME;
//			user->Send((char*)&send_packet, send_packet.size);
//		}
//	}
//	else
//		return false;
//
//	return true;
//}
//bool CRoom::SendOhterUser(CUserSession * user, void* pPacket, DWORD size)
//{
//	CThreadSync Sync;
//	if (IsRoomUser(user))
//	{
//		for (iterMap = useMapList.begin(); iterMap != useMapList.end(); iterMap++)
//		{
//			if (iterMap->second == user)
//				continue;
//			user->Send((char*)pPacket, size);
//		}
//	}
//	else
//		return false;
//
//	return true;
//}

//�濡 ���� ���� ����
bool CRoom::IsRoomUser(CUserSession *user) 
{
	iterMap = useMapList.find(user->GetSessionSock());
	if (iterMap != useMapList.end())
	{
		cout << "���� �� �ȿ� �����ϰ� �ֽ��ϴ�." << endl;
		return true;
	}
	else
	{
		//CCLog::WriteLog(_T("Is Not Room User"));
		return false;
	}

}

CUserSession* CRoom::GetTwoRoomOtherUser(CUserSession * user)
{

	for (iterMap = useMapList.begin(); iterMap != useMapList.end(); iterMap++)
	{
		if (iterMap->second == user)	
			continue;
		else
		{
			return iterMap->second;
		}
	}
	if (iterMap != useMapList.end())
	{
		cout << "���� �� �ȿ� �������� ����" << endl;
	}
	return iterMap->second;
}

//������Ʈ
VOID CRoom::Update(CUserSession *user, void * pPacket, DWORD number, bool correct)
{
	switch (number)
	{
	case JOIN:
		user_num++;
		if (user_num == MAX_USER)
		{
			m_full = true;
			cout << "Ǯ��" << endl;
		}
		cout << "���� ����" << endl;
		break;
	case LEAVE:
		if (user_num >= 0)
		{
			user_num--;
			m_full = false;
		}
		cout << "���� ����" << endl;
		break;
	}
}