# eat-client-iocp-server1
-----
![오목게임 깃허브 리드미1](https://user-images.githubusercontent.com/86718283/124387811-ea04bf80-dd1a-11eb-844d-3274a6ee05c0.png)

----------------
>사용언어
```
c / c ++ / win32
```
----------------
>서버
```
iocp server
```
+ CrirticalSection.h
+ CDatabase.h
+ CCircleQueue.h
+ CResult.h
+ CSingleTon.h
+ protocal.h
+ stdafx.h
+ CLog.h
+ CLogin.h
+ CRoom.h
+ CRoomManager.h
+ CIocp.h
+ CServer.h
+ CSession.h
+ CSessionManager.h
+ CUserManager.h
+ CUserSession.h
+ CThread.h

>> main 함수

![image](https://user-images.githubusercontent.com/86718283/124422520-6096d100-dd9e-11eb-8b1f-8fc679e11903.png)
1. database 연결
2. ListenBind 연결
3. iocp 연결
4. SESSION 초기화 (userManager 생성, RoomManager 생성)
5. CreateSeccsion ( 사용, AcceptEx 접속받기)

![image](https://user-images.githubusercontent.com/86718283/124497846-746e2180-ddf6-11eb-9e4c-fb4e3acb51ce.png)

6. WSARecv
7. CircleQueue 
8. Parsing
9. CircleQueue
10. WSASend


>>CDatabase.h

![image](https://user-images.githubusercontent.com/86718283/124423297-cafc4100-dd9f-11eb-8ea9-f86fbfc5b3ef.png)
1. ConnectSql()       //(db연결, 한글 설정)
2. 그 외 set, get 함수 

>>CServer.h

![image](https://user-images.githubusercontent.com/86718283/124423539-5544a500-dda0-11eb-90ab-60d8e18d8e4f.png)
1. InitServer()       //(윈속초기화, 소켓 초기화)
2. ListenBindServer() //(Listen, Bind, acceptEx 하기위한 iocp 연결)
3. 그 외 get 함수

>>CIocp.h

![image](https://user-images.githubusercontent.com/86718283/124423705-ae143d80-dda0-11eb-96f7-475e2fb4a8ad.png)
1. CreateIocp()               //(iocp생성 함수)
2. ConnectIocp_ListenSocket() //(acceptEx 하기위한 iocp 연결 함수)
3. ConnectIocp_Session()      //(Session와 CreateIoCompletionPort 연결)

>>CLogin.h

![image](https://user-images.githubusercontent.com/86718283/124430275-ee2bee00-dda9-11eb-8b28-a14329798b05.png)
1. CompareUserID()  //(회원가입 시 중복 아이디 여부 db로 확인하는 함수)
2. CreateUserID()   //(db에 아이디 생성하는 함수)
3. LoginUser()      //(db에 등록된 id,pw 확인하여 로그인하는 함수)
4. GetNickname()    //(db에 등록된 id 값과 비교하여 nickname 값 얻어오는 함수)
5. GetCharacterNum()//(db에 등록된 id 값과 비교하여 characterNum 값 얻어오는 함수)
6. DeleteUserID()   //(db에 등록된 유저 정보 삭제하는 함수)

>>CRoomManager.h

![image](https://user-images.githubusercontent.com/86718283/124430837-a063b580-ddaa-11eb-9d87-9c10350b2ab2.png)
1. CreateRoom()    //(room 생성 함수)
2. DestroyRoom()   //(room 제거 함수)
3. GetRoomCount()  //(room의 총갯수 반환함수)
4. GetRoomList()   //(room의 목록을 받아오는 함수)

>>CRoom.h

![image](https://user-images.githubusercontent.com/86718283/124431146-fa647b00-ddaa-11eb-8e7e-0e54829a6426.png)
1. Update()               //(join, leave 상태에 맞게 유저를 처리하는 함수)
2. Set()                  //(check 값을 설정하는 함수)
3. GetUserNum()           //(접속한 유저 전체 유저수를 받아오는 함수)
4. IsFuLL()               //(room이 꽉찼는지 확인하는 함수)
5. JoinUser()             //(접속한 유저 room에 입장시키는 함수)
6. LeaveUser()            //(room을 떠난 유저를 처리하는 함수)
7. Broadcast_User()       //(같은 room 에 존재하는 유저들에게 모두 전송하는 함수)
8. IsRoomUser()           //(room 에 유저가 있는지 확인하는 함수)
9. GetTwoRoomOtherUser()  //(2인 room의 경우 상대방을 확인하는 함수)
10. GetuseMapList()       //(userMapList를 받아오는 함수)

>>CSessionManager.h

![image](https://user-images.githubusercontent.com/86718283/124424112-6d68f400-dda1-11eb-9436-a97d01811415.png)
1. Init() (CUserManager, CRoomManger 생성)
2. CreateSeccsion() (session 생성, acceptEx 접속받기 객체 풀링 사용)
3. ActiveSession() (객체풀링 활성화 함수)
4. InActiveSession() (객체풀링 비활성화 함수)
5. 그 외 get 함수

>>CSession.h

![image](https://user-images.githubusercontent.com/86718283/124432041-1c123200-ddac-11eb-8123-e49c38836dc4.png)
1. GetSessionSock()    //(SessionSock 을 받아오는 함수)
2. AcceptExSession()   //(acceptEx를 사용하여 비동기 접속 요청을 처리하는 함수)
3. GetAcceptINFO()     //(acceptEx를 통해 접속한 클라이언트의 주소를 받아오는 함수)
4. CloseSocket()       //(소켓을 재활용하는 함수)
5. PacketParsing()     //(패킷을 파싱하는 함수)
6. InVaildPacket()     //(패킷 확인 함수)
7. PreOnRecv()         //(WSARecv함수를 통해 받아 CircleQueue에 저장된 값을 꺼내 PacketParsing하는 함수)
8. Recv()              //(WSARecv함수로 데이터를 받아 CircleQueue에 저장하는 함수)
9. Send()              //(WSASend함수를 통해 데이터를 CircleQueue에 보낸 후 전송하는 함수)
10. 그외 get 함수

>>CUserManager.h

![image](https://user-images.githubusercontent.com/86718283/124433092-4f08f580-ddad-11eb-851c-8eb1d640078d.png)
1. Enter_User()      //(접속한 유저를 처리하는 함수)
2. Leave_User()      //(연결 종료한 유저를 처리하는 함수)
3. 그외 get 함수

>>CUserSession.h

![image](https://user-images.githubusercontent.com/86718283/124433305-9099a080-ddad-11eb-8bff-574969b71d03.png)
1. PacketParsing()     //(패킷을 파싱하여 처리하는 함수)
2. 패킷처리 함수들

>>CThread.h

![image](https://user-images.githubusercontent.com/86718283/124424399-ea946900-dda1-11eb-8d6f-5d3a3b5e1cf5.png)
1. CreateThread() (workerThread 생성 (GetSystemInfo*2))
2. DestroyThread() (workerThread 제거)
3. WorkerThread() (GetQueuedCompletionStatus을 통하여 데이터 처리)

>>CriticalSection.h

![image](https://user-images.githubusercontent.com/86718283/124433524-cfc7f180-ddad-11eb-9cb8-439d28661ea0.png)
1. 크리티컬세션

>>CircleQueue.h

![image](https://user-images.githubusercontent.com/86718283/124433676-fc7c0900-ddad-11eb-99f1-b756a8f40cbe.png)
![image](https://user-images.githubusercontent.com/86718283/124433733-0b62bb80-ddae-11eb-9229-1b9808430bea.png)
![image](https://user-images.githubusercontent.com/86718283/124433781-174e7d80-ddae-11eb-85b7-e0e84935d2bb.png)
1. circleQueue      //(패킷을 recv, send 하기 전 값을 저장하는 queue)

>>CResult.h

![image](https://user-images.githubusercontent.com/86718283/124433969-482eb280-ddae-11eb-8c5e-585d59f3142e.png)
1. GetOtherUserResult()     //(다른 유저들의 nickname과 승리 값울 db를 통해 받아오는 함수)
2. GetMyResult()            //(현재 유저의 승리 값을 db에서 받아오는 함수)
3. IncreaseWinResult()      //(승리한 유저의 db에 승리 값 추가하는 함수)
4. GetUserNum()             //(db에 등록된 유저수 받아오는 함수)

>>CSingleton.h

![image](https://user-images.githubusercontent.com/86718283/124434313-b2475780-ddae-11eb-9a45-0e830d410bee.png)
1. 싱글톤 함수

>>Protocal.h

![image](https://user-images.githubusercontent.com/86718283/124434417-d2771680-ddae-11eb-91f5-b461bd04ef6e.png)
![image](https://user-images.githubusercontent.com/86718283/124434461-de62d880-ddae-11eb-99d3-12e58d8c920c.png)
![image](https://user-images.githubusercontent.com/86718283/124434495-e91d6d80-ddae-11eb-83e2-2012b71483bb.png)
![image](https://user-images.githubusercontent.com/86718283/124434528-f20e3f00-ddae-11eb-80ee-0a56c39c05d1.png)
![image](https://user-images.githubusercontent.com/86718283/124434804-46b1ba00-ddaf-11eb-9227-d016f2dcd1b5.png)
![image](https://user-images.githubusercontent.com/86718283/124434835-50d3b880-ddaf-11eb-8a3f-33d23eb02695.png)
![image](https://user-images.githubusercontent.com/86718283/124434868-5b8e4d80-ddaf-11eb-86ea-cca580adadfe.png)
![image](https://user-images.githubusercontent.com/86718283/124434910-66e17900-ddaf-11eb-8074-99324346c9b9.png)
![image](https://user-images.githubusercontent.com/86718283/124434939-6ea11d80-ddaf-11eb-8b52-ffba3f334c74.png)
1. 패킷 함수

----------------
>클라이언트
```
win32
```
+ singleton.h
+ fivetree.h
+ stdafx.h
+ protocal.h
+ sendPacket.h
+ server.h
+ dialog.h
+ gameWindow.h
+ loginWindow.h
+ resultWindow.h
+ waitRoomWindow.h

-----------------

>> 로그인 구현

![image](https://user-images.githubusercontent.com/86718283/124473188-f8fe7700-ddd9-11eb-9bd4-3158f7bce1f5.png)
1. 회원 가입 구현 (다이얼로그 실행)
2. 로그인 구현

>> 로비 구현

![image](https://user-images.githubusercontent.com/86718283/124473633-86da6200-ddda-11eb-99a5-987dc7731d7e.png)

1. 채팅창 구현
2. 대기실 유저 nickname 구현
3. 랭킹 전적 구현
4. 애니메이션으로 케릭터 움직임 구현

>> 게임 구현

![image](https://user-images.githubusercontent.com/86718283/124473801-b4bfa680-ddda-11eb-81b3-4636066b7ee6.png)

1. 채팅창 구현
2. 유저 케릭터, 이름 정보 구현
3. 오목 게임 구현

![image](https://user-images.githubusercontent.com/86718283/155486048-5e212ea3-e1aa-41df-9775-f2c3a47ef2b8.png)
![image](https://user-images.githubusercontent.com/86718283/155486095-37ebee66-a3f2-47a7-9cdd-48b422737509.png)
![image](https://user-images.githubusercontent.com/86718283/155486147-32ad12e3-ffc9-4666-89bb-1b08c4b59ec2.png)
![image](https://user-images.githubusercontent.com/86718283/155486179-4185809b-e518-4cb3-bd7a-039dbc8c22e1.png)
![image](https://user-images.githubusercontent.com/86718283/155486215-eace8c60-db93-4237-ad63-52aae78ef3fa.png)
![image](https://user-images.githubusercontent.com/86718283/155486244-80c6bea4-6712-498c-81de-5cd6e0716eb0.png)
![image](https://user-images.githubusercontent.com/86718283/155486280-9587e7f6-3b1e-4fb3-b948-489fd309fc46.png)
![image](https://user-images.githubusercontent.com/86718283/155486309-932039b8-2d02-402f-961d-e767624d2890.png)
