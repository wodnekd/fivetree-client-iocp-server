#include "stdafx.h"

CUserSession::CUserSession()
{
	packetMap.insert(packetList::value_type(TEST, &CUserSession::Test_PACKET));
	packetMap.insert(packetList::value_type(SIGNUP_REQ, &CUserSession::SignUp_REQ));
	packetMap.insert(packetList::value_type(SIGNUP_ACK, &CUserSession::SignUp_ACK));
	packetMap.insert(packetList::value_type(CHECK_ID_REQ, &CUserSession::CheckID_REQ));
	packetMap.insert(packetList::value_type(CHECK_ID_ACK, &CUserSession::CheckID_ACK));
	packetMap.insert(packetList::value_type(LOGIN_USER_REQ, &CUserSession::LoginUser_REQ));
	packetMap.insert(packetList::value_type(LOGIN_USER_ACK, &CUserSession::LoginUser_ACK));
	packetMap.insert(packetList::value_type(SEND_NICKNAME_REQ, &CUserSession::SendNickname_REQ));
	packetMap.insert(packetList::value_type(SEND_NICKNAME_ACK, &CUserSession::SendNickname_ACK));

	packetMap.insert(packetList::value_type(SEND_OUT_NICKNAME_REQ, &CUserSession::SendOutNickname_REQ));
	packetMap.insert(packetList::value_type(SEND_OUT_NICKNAME_ACK, &CUserSession::SendOutNickname_ACK));

	packetMap.insert(packetList::value_type(ENTER_FIVETREE_GAME_REQ, &CUserSession::EnterFivetreeGame_REQ));
	packetMap.insert(packetList::value_type(ENTER_FIVETREE_GAME_ACK, &CUserSession::EnterFivetreeGame_ACK));
	packetMap.insert(packetList::value_type(OUT_FIVETREE_GAME_REQ, &CUserSession::OutFivetreeGame_REQ));
	packetMap.insert(packetList::value_type(OUT_FIVETREE_GAME_ACK, &CUserSession::OutFivetreeGame_ACK));
	packetMap.insert(packetList::value_type(START_FIVETREE_GAME_REQ, &CUserSession::StartFivetreeGame_REQ));
	packetMap.insert(packetList::value_type(START_FIVETREE_GAME_ACK, &CUserSession::StartFivetreeGame_ACK));
	packetMap.insert(packetList::value_type(FIVETREE_PUTSTONE_REQ, &CUserSession::FivetreePutstone_REQ));
	packetMap.insert(packetList::value_type(FIVETREE_PUTSTONE_ACK, &CUserSession::FivetreePutstone_ACK));

	packetMap.insert(packetList::value_type(CHAT_REQ, &CUserSession::Chat_REQ));
	packetMap.insert(packetList::value_type(CHAT_ACK, &CUserSession::Chat_ACK));
	packetMap.insert(packetList::value_type(ROOM_CHAT_REQ, &CUserSession::RoomChat_REQ));
	packetMap.insert(packetList::value_type(ROOM_CHAT_ACK, &CUserSession::RoomChat_ACK));

	packetMap.insert(packetList::value_type(WIN_TOTAL_REQ, &CUserSession::WinTotal_REQ));
	packetMap.insert(packetList::value_type(WIN_TOTAL_ACK, &CUserSession::WinTotal_ACK));

	packetMap.insert(packetList::value_type(EXIT_REQ, &CUserSession::Exit_REQ));
	packetMap.insert(packetList::value_type(EXIT_ACK, &CUserSession::Exit_ACK));
	
	state = LOGIN;
	memset(userID, 0, sizeof(userID));

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = 3;
		}
	}
}

CUserSession::~CUserSession()
{
	packetMap.clear();
	SESSION.GetRoomManager()->GetRoomList()[0]->LeaveUser(this);
}
//��Ŷ �Ľ�

VOID CUserSession::PacketParsing(PacketHeader *pPacket)
{
	if (pPacket == NULL)
	{
		cout << "In Not Packet" << endl;
		return;
	}
	//��Ŷ�ʿ��� ���� ���� ��Ŷ�� �ڵ带 ã�� iter�� �־��ش�
	iter = packetMap.find(pPacket->code);
	//�Լ� �����͸� �̿��Ͽ� ��Ŷ�Լ� ����
	if (iter != packetMap.end())
	{
		pF Fp = iter->second; 
		(this->*Fp)(pPacket);
	}
}

VOID CUserSession::Test_PACKET(PacketHeader * pPacket)
{
	cout << "aaa" << endl;
	PACKET_TEST *test = new PACKET_TEST;
	Send((char*)test, test->size);
}

VOID CUserSession::SignUp_REQ(PacketHeader *pPacket)
{
	//�Ľ̵� ��Ŷ�� ����
	PACKET_SIGNUP_REQ *packet = (PACKET_SIGNUP_REQ*)pPacket;
	//���� ��Ŷ ����
	PACKET_SIGNUP_ACK send_packet;
	state = LOGIN;
	if (!Login.CreateUserID(packet->createID, packet->createPW, packet->createNickname, packet->characterNum))
	{
		send_packet.signupSuccess = false;
		cout << "���̵� ���� ����" << endl;
		CCLog::WriteLog(_T("LoginFaill"));
	}
	else
	{
		send_packet.signupSuccess = true;
		printf("���̵� : [ %S ] �г��� : [ %S ] \n", packet->createID, packet->createNickname);
		cout << endl;
	}
	//��Ŷ ������
	Send((char*)&send_packet, send_packet.size);
}
VOID CUserSession::SignUp_ACK(PacketHeader * pPacket)
{
	cout << "NewUer_ACK" << endl;
}


VOID CUserSession::CheckID_REQ(PacketHeader * pPacket)
{
	PACKET_CHECKID_REQ *recv_packet = (PACKET_CHECKID_REQ*)pPacket;
	PACKET_CHECKID_ACK send_packet;
	state = LOGIN;
	if (Login.CompareUserID(recv_packet->loginID))
	{
		send_packet.checkIdSuccess = false;
		cout << "�ߺ� �� ���̵�� ���� ����" << endl;
	}
	else
	{
		send_packet.checkIdSuccess = true;
		cout << "���̵� ���� ����" << endl;
	}
	Send((char*)&send_packet, send_packet.size);
}

VOID CUserSession::CheckID_ACK(PacketHeader * pPacket)
{
	cout << "NewUer_ACK" << endl;
}

//�α��� ó�� ��Ŷ
VOID CUserSession::LoginUser_REQ(PacketHeader * pPacket)
{
	PACKET_LOGIN_USER_REQ *recv_packet = (PACKET_LOGIN_USER_REQ*)pPacket;
	PACKET_LOGIN_USER_ACK send_packet; 
	state = LOGIN;
	//�α��� ���� �� ó��
	if (!Login.LoginUser(recv_packet->loginID, recv_packet->loginPW))
	{
		send_packet.loginUserSuccess = false;
		cout << "�α��� ����" << endl;
	}
	//�α��� ���� �� ó��
	else
	{
		//���̵�, �г��� ���� ����
		wcscpy(this->userID, recv_packet->loginID);
		wcscpy(this->nickname, Login.GetNickname(recv_packet->loginID));
		this->characterNum = Login.GetCharacterNum(recv_packet->loginID);
		state = LOBBY;
		//UserManager�� ���� ���� ����
		SESSION.GetUserManager()->Enter_User(this);
		//�游��� ù �����ڰ� �� �����
		if(SESSION.GetRoomManager()->GetRoomCount() == 0)
		{
			SESSION.GetRoomManager()->CreateRoom();
		}
		//��� �� ���� (���� �� �ε��� 0��)
		SESSION.GetRoomManager()->GetRoomList()[0]->JoinUser(this);
		gameRoomNumber = 0;
		//������ ���� ��, ���� ���� �� Ȯ��
		cout << "���� �� : " << SESSION.GetUserManager()->GetClientNum() << endl;
		cout << "���� ���� �� : " << SESSION.GetRoomManager()->GetRoomList()[0]->GetUserNum() << endl;
		//send ��Ŷ ���� ���� (�г���, �ɸ��� ��ȣ, �α��� ����)
		wcscpy(send_packet.nickname, this->nickname);
		send_packet.characterNum = Login.GetCharacterNum(recv_packet->loginID);
		send_packet.loginUserSuccess = true;
		cout << "�α��� ����" << endl;
	}
	//��Ŷ ����
	Send((char*)&send_packet, send_packet.size);
}

VOID CUserSession::LoginUser_ACK(PacketHeader * pPacket)
{
	cout << "LoginUser_ACK" << endl;
}

//�ٸ� �����鿡�� ������ ������ �г����� �����ִ� ��Ŷ
VOID CUserSession::SendNickname_REQ(PacketHeader * pPacket)
{
	PACKET_SEND_NICKNAME_REQ *recv_packet = (PACKET_SEND_NICKNAME_REQ*)pPacket;
	PACKET_SEND_NICKNAME_ACK send_packet;
	wcscpy(send_packet.nickname, recv_packet->nickname);
	
	//���� ������ ���� �г����� ������ �ٸ� �����鿡�� �����ش�.
	//1.
	//SESSION.GetRoomManager()->GetRoomList()[this->RoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);
	SESSION.GetRoomManager()->GetRoomList()[0]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);

	map<SOCKET, CUserSession*> m = SESSION.GetUserManager()->GetOnList();
	map<SOCKET, CUserSession*>::iterator iter;
	//�ٸ� �������� �г����� ���� ������ �������� �����ش�.
	wcscpy(send_packet.nickname, L"");
	for (iter = m.begin(); iter != m.end(); iter++)
	{
		if (this != iter->second)
		{
			wcscpy(send_packet.nickname, iter->second->nickname);
			Send((char*)&send_packet, send_packet.size);
		}
		else
		{
			continue;
		}
	}
}

VOID CUserSession::SendNickname_ACK(PacketHeader * pPacket)
{
	cout << "SendID_ACK" << endl;
}
//1.
VOID CUserSession::SendOutNickname_REQ(PacketHeader * pPacket)
{
	PACKET_SEND_OUT_NICKNAME_REQ *recv_packet = (PACKET_SEND_OUT_NICKNAME_REQ*)pPacket;
	PACKET_SEND_OUT_NICKNAME_ACK send_packet;
	wcscpy(send_packet.nickname, recv_packet->nickname);

	SESSION.GetRoomManager()->GetRoomList()[0]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);
}
//1.
VOID CUserSession::SendOutNickname_ACK(PacketHeader * pPacket)
{
	cout << "SendOtherID_ACK" << endl;
}

//���ӹ� ���� ��Ŷ
VOID CUserSession::EnterFivetreeGame_REQ(PacketHeader * pPacket)
{
	PAKCET_ENTER_FIVETREE_GAME_ACK send_packet;
	//�� ��ü Ȯ�� 1����
	for (int roomNum = 1; roomNum < MAX_ROOMCOUNT; roomNum++)
	{
		//1������� ���� ���� �� Ȯ��
		int userNum = userNum = SESSION.GetRoomManager()->GetRoomList()[roomNum]->GetUserNum();
		//������ 0���̸� ���� userNum�� ����
		if (userNum == 0)
		{
			//���ӹ� ��ȣ ���ǿ� ����
			gameRoomNumber = roomNum;
			homeAway = HOME;
			//���� roomNum ���ӹ濡 ���� ����
			SESSION.GetRoomManager()->GetRoomList()[roomNum]->JoinUser(this);
			//���� ��Ŷ ���� ���� (���� ������ ������ �г���, �ɸ��� ��ȣ, home, �� ��ȣ)
			wcscpy(send_packet.nickname, this->nickname);
			send_packet.characterNum = this->characterNum;
			send_packet.homeAway = HOME;
			//Ȯ��
			cout <<"userNum == 0 // GameRoomNumber : "<< gameRoomNumber << endl;
			//������
			Send((char*)&send_packet, send_packet.size);
			return;
		}
		//������ ������ 1�� �ִٸ� (2�ι��̹Ƿ� ���� ����)
		else if (userNum == 1)
		{
			//���ӹ� ��ȣ ���ǿ� ����
			gameRoomNumber = roomNum;
			homeAway = AWAY;
			//roomNum�� ����
			SESSION.GetRoomManager()->GetRoomList()[roomNum]->JoinUser(this);
			//������ �ִ� ������ ���� �����޾ƿ��� (������ �ִ� ������ ������ this�������� ������ ����)
			CUserSession* twoRoomOtherUser = SESSION.GetRoomManager()->GetRoomList()[roomNum]->GetTwoRoomOtherUser(this);
			//1.1.		
			//������ �ִ� ���� ���� ���� �� this �������� ����
			PAKCET_ENTER_FIVETREE_GAME_ACK send_packet;
			wcscpy(send_packet.nickname, twoRoomOtherUser->nickname);
			send_packet.characterNum = twoRoomOtherUser->characterNum;
			send_packet.size = sizeof(send_packet);
			send_packet.homeAway = HOME;
			//������ �ִ� ���� ���� ����
			Send((char*)&send_packet, send_packet.size);

			//this ������ ���� ��Ŷ�� ���� (���� ������ ������ �г���, �ɸ��� ��ȣ, away, �溡ȣ)
			wcscpy(send_packet.nickname, this->nickname);
			send_packet.characterNum = this->characterNum;
			send_packet.homeAway = AWAY;
			//Ȯ��
			cout << "userNum == 1 // gameRoomNumber : " << gameRoomNumber << endl;
			//1.
			SESSION.GetRoomManager()->GetRoomList()[roomNum]->Broadcast_User(this, (void *)&send_packet, send_packet.size, true);
			
			return;
		}
		//������ ������ 2�� �ִٸ� ���� ���� ���̹Ƿ� ���� ���� Ȯ��
		else
		{
			continue;
		}
	}
}

VOID CUserSession::EnterFivetreeGame_ACK(PacketHeader * pPacket)
{
	cout << "EnterFivetreeGame_ACK" << endl;
}
VOID CUserSession::OutFivetreeGame_REQ(PacketHeader * pPacket)
{
	//PACKET_OUT_FIVETREE_GAME_REQ *recv_packet = (PACKET_OUT_FIVETREE_GAME_REQ*)pPacket;
	PACKET_OUT_FIVETREE_GAME_ACK send_packet;
	send_packet.homeAway = homeAway;
	cout << "���� homeAway : " << send_packet.homeAway << endl;
	cout << "OutFivetreeGame_REQ ���� �� ��ȣ : " << gameRoomNumber <<endl;

	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->LeaveUser(this, &send_packet, send_packet.size);
	//���ǿ� ����� ���ӹ� ��ȣ �ʱ�ȭ
	gameRoomNumber = 0;
}

VOID CUserSession::OutFivetreeGame_ACK(PacketHeader * pPacket)
{
	cout << "OutFivetreeGame_ACK" << endl;
}

VOID CUserSession::StartFivetreeGame_REQ(PacketHeader * pPacket)
{
	//PACKET_START_FIVETREE_GAME_REQ* recv_packet = (PACKET_START_FIVETREE_GAME_REQ*)pPacket;
	PACKET_START_FIVETREE_GAME_ACK send_packet;

	send_packet.putStone = true;
	send_packet.blackWhite = black;
	Send((char*)&send_packet, send_packet.size);
	
	send_packet.putStone = false;
	send_packet.blackWhite = white;

	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);

}

VOID CUserSession::StartFivetreeGame_ACK(PacketHeader * pPacket)
{
	cout << "StartFivetreeGame_ACK" << endl;
}

VOID CUserSession::FivetreePutstone_REQ(PacketHeader * pPacket)
{
	PACKET_FIVETREE_PUTSTONE_REQ* recv_packet = (PACKET_FIVETREE_PUTSTONE_REQ*)pPacket;
	PACKET_FIVETREE_PUTSTONE_ACK send_packet;
	//���忡 �ٵϵ� ���� ��ġ �Է�
	//1.
	board[recv_packet->x / 50][recv_packet->y / 50] = recv_packet->blackWhite;
	
	//���� �������� ������
	send_packet.turn = true;
	send_packet.x = recv_packet->x;
	send_packet.y = recv_packet->y;
	send_packet.blackWhite = recv_packet->blackWhite;
	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);

	//�¸� ����
	if (CheckWin(recv_packet->blackWhite) == true)
	{
		if (recv_packet->blackWhite == 0)
		{
			cout << "������ �¸�" << endl;
		}
		else
		{
			cout << "�Ͼᵹ �¸�" << endl;
		}
		//������ �¸� �߰�
		Result.IncreaseWinResult(userID);
		InitBoard();
		PACKET_FIVETREE_RESULT_ACK send_packet;
		send_packet.winLose = true;
		Send((char*)&send_packet, send_packet.size);
		send_packet.winLose = false;
		SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);
	}
}

void CUserSession::FivetreePutstone_ACK(PacketHeader * pPacket)
{
	cout << "FivetreePutstone_ACK" << endl;
}



VOID CUserSession::Chat_REQ(PacketHeader * pPacket)
{
	PACKET_CHAT_REQ *recv_packet = (PACKET_CHAT_REQ*)pPacket;
	PACKET_CHAT_ACK send_packet;
	printf("���ǿ��� ���� �޽��� ���� : %S\n", recv_packet->chat);
	cout << "���� �ڵ� : " << recv_packet->code << endl;
	swprintf(send_packet.chat, 20, L"%s : %s", nickname, recv_packet->chat);
	SESSION.GetRoomManager()->GetRoomList()[0]->Broadcast_User(this, (void *)&send_packet, send_packet.size, true);
}

VOID CUserSession::Chat_ACK(PacketHeader * pPacket)
{
	cout << "Chat_ACK" << endl;
}

VOID CUserSession::RoomChat_REQ(PacketHeader * pPacket)
{
	PACKET_ROOM_CHAT_REQ *recv_packet = (PACKET_ROOM_CHAT_REQ*)pPacket;
	PACKET_ROOM_CHAT_ACK send_packet;
	cout << "���ӹ� ��ȣ : "<< gameRoomNumber << endl;
	cout << "���ӹ� �ڵ� : " << recv_packet->code << endl;
	printf("�濡�� ���� �޽��� ���� : %S\n", recv_packet->chat);
	swprintf(send_packet.chat, 20, L"%s : %s", nickname, recv_packet->chat);
	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, true);
}

VOID CUserSession::RoomChat_ACK(PacketHeader * pPacket)
{
	cout << "Chat_ACK" << endl;
}

VOID CUserSession::WinTotal_REQ(PacketHeader * pPacket)
{
	PACKET_WIN_TOTAL_REQ *recv_packet = (PACKET_WIN_TOTAL_REQ*)pPacket;
	PACKET_WIN_TOTAL_ACK send_packet;
	int num = Result.GetUserNum();
	for (int i = 0; i < num; i++)
	{
		Sleep(10);
		wcscpy(send_packet.nickName, Result.GetOtherUserResult(i));
		Send((char*)&send_packet, send_packet.size);
	}
	PACKET_WIN_MY_TOTAL_ACK send_packet2;
	wcscpy(send_packet2.nickName, Result.GetMyResult(nickname));
	Send((char*)&send_packet2, send_packet.size);
}

VOID CUserSession::WinTotal_ACK(PacketHeader * pPacket)
{
	return VOID();
}





VOID CUserSession::Exit_REQ(PacketHeader* pPacket)
{
	SESSION.GetRoomManager()->GetRoomList()[0]->LeaveUser(this);
	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->LeaveUser(this);
	SESSION.GetUserManager()->Leave_User(this);
	cout << "���� ���� �� : " << SESSION.GetRoomManager()->GetRoomList()[0]->GetUserNum() << endl;
	cout << "���� ���� �� : " << SESSION.GetUserManager()->GetClientNum() << endl;
	
}

VOID CUserSession::Exit_ACK(PacketHeader * pPacket)
{
	cout << "Exit_ACK" << endl;
}

void CUserSession::InitBoard()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = 3;
		}
	}
}

bool CUserSession::CheckWin(bool blackWhite)
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] == blackWhite && board[i][j + 1] == blackWhite && board[i][j + 2] == blackWhite && board[i][j + 3] == blackWhite && board[i][j + 4] == blackWhite)
			{
				return true;
			}
			else if (board[i][j] == blackWhite && board[i + 1][j] == blackWhite && board[i + 2][j] == blackWhite && board[i + 3][j] == blackWhite && board[i + 4][j] == blackWhite)
			{
				return true;
			}
			else if (board[i][j] == blackWhite && board[i + 1][j + 1] == blackWhite && board[i + 2][j + 2] == blackWhite && board[i + 3][j + 3] == blackWhite && board[i + 4][j + 4] == blackWhite)
			{
				return true;
			}
			else if (board[i][j] == blackWhite && board[i - 1][j + 1] == blackWhite && board[i - 2][j + 2] == blackWhite && board[i - 3][j + 3] == blackWhite && board[i - 4][j + 4] == blackWhite)
			{
				return true;
			}
		}
	return false;
}
