#pragma once
enum UserState
{
	LOGIN = 0, LOBBY, GAME
};


class CUserSession : public CSession
{
private:
	typedef VOID(CUserSession::*pF)(PacketHeader *pPacket);	 
	typedef std::map<DWORD, pF>	packetList;					
	packetList					packetMap;
	packetList::iterator		iter;
	UserState state;
	WCHAR userID[USER];
	WCHAR nickname[20];
	int characterNum;
	bool homeAway;
	int	gameRoomNumber;
	int board[10][10];
public:
	CUserSession();
	~CUserSession();

	
	VOID PacketParsing(PacketHeader *pPacket);
	///////////
	VOID Test_PACKET(PacketHeader *pPacket);

	VOID SignUp_REQ(PacketHeader *pPacket);
	VOID SignUp_ACK(PacketHeader *pPacket);
	VOID CheckID_REQ(PacketHeader *pPacket);
	VOID CheckID_ACK(PacketHeader *pPacket);
	VOID LoginUser_REQ(PacketHeader *pPacket);
	VOID LoginUser_ACK(PacketHeader *pPacket);

	VOID SendNickname_REQ(PacketHeader *pPacket);
	VOID SendNickname_ACK(PacketHeader *pPacket);
	VOID SendOutNickname_REQ(PacketHeader *pPacket);
	VOID SendOutNickname_ACK(PacketHeader *pPacket);

	VOID EnterFivetreeGame_REQ(PacketHeader *pPacket);
	VOID EnterFivetreeGame_ACK(PacketHeader *pPacket);
	VOID OutFivetreeGame_REQ(PacketHeader *pPacket);
	VOID OutFivetreeGame_ACK(PacketHeader *pPacket);
	VOID StartFivetreeGame_REQ(PacketHeader *pPacket);
	VOID StartFivetreeGame_ACK(PacketHeader *pPacket);
	VOID FivetreePutstone_REQ(PacketHeader *pPacket);
	void FivetreePutstone_ACK(PacketHeader *pPacket);

	VOID Chat_REQ(PacketHeader *pPacket);
	VOID Chat_ACK(PacketHeader *pPacket);
	VOID RoomChat_REQ(PacketHeader *pPacket);
	VOID RoomChat_ACK(PacketHeader *pPacket);

	VOID WinTotal_REQ(PacketHeader *pPacket);
	VOID WinTotal_ACK(PacketHeader *pPacket);

	VOID Exit_REQ(PacketHeader *pPacket);
	VOID Exit_ACK(PacketHeader *pPacket);

	void InitBoard();
	bool CheckWin(bool blackWhite);
	UserState GetUserState() const { return state; }
};