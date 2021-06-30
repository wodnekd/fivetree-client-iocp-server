#pragma once

template< class T >
class CCircleQueue
{
private:
   T*         m_Buffer;               //데이터를 저장할 공간
   DWORD      m_BufferSize;            //그 공간의 크기
   DWORD      m_DataSize;               //데이터의 크기
   DWORD      m_PosHead;               //Head
   DWORD      m_PosTail;               //Tail

public:
   CCircleQueue() 
   {
      m_Buffer = 0;
      m_DataSize = 0;
      m_BufferSize = 0;
      m_PosHead = 0;
      m_PosTail = 0;
   };

   ~CCircleQueue()
   {
      Destroy();
   };
   // 데이터, 헤드, 테일 초기화
   void Clear() 
   {
      m_DataSize = 0;
      m_PosHead  = 0;
      m_PosTail  = 0;
   };
   //버퍼 해제
   void Destroy()
   {
      if (m_Buffer != 0) 
      {
         delete[] m_Buffer;
      }
      m_Buffer = 0;
      Clear();
   };
   //버퍼 크기 생성
   BOOL Create(DWORD BufferSize) 
   {
      if (m_Buffer != 0)   return FALSE;

      m_Buffer      = new T[BufferSize];         //입력된bufferSize 만큼 버퍼 생성
      m_BufferSize   = BufferSize;               //입력된버퍼 크기 저장
      memset(m_Buffer, 0, sizeof(m_Buffer));         //버퍼를 0으로 초기화
      Clear();                              //버퍼 사용에 필요한 내부 변수 초기화
      return TRUE;
   };
   //버퍼에 데이터 입력
   bool Enqueue(T* Data, DWORD Size) 
   {
      //버퍼에 저장할 공간이 없으면 실패 저장 불가
      if (GetRemainBufSize() < Size) return FALSE;
      //Data 값을 버퍼에 저장
      if (Data != 0)
      {
         //초기 시작, H
         if (m_PosHead <= m_PosTail)
         {
            //남은 버퍼 크기 RemainSize에 저장
            DWORD      RemainSize = m_BufferSize - m_PosTail;

            //  남은 버퍼 크기가 들어온 데이터 크기보다 크다면 한번에 복사한다.
            if (RemainSize >= Size)
            {
               //m_Buffer 처음 위치 + m_PosTail(입력되어진 위치)
               memcpy
               (
                  m_Buffer + m_PosTail,               //복사를 당하는 주소 
                  Data,                           //복사를 하는 주소
                  sizeof(T) * Size                  //복사할 바이트 갯수
               );
            }
            else
            {
               //  버퍼에 여유가 없다면 데이터를 두개로 나누어
               //  버퍼의 앞/뒤에 복사한다.
               memcpy(m_Buffer + m_PosTail, Data, sizeof(T) * RemainSize);
               memcpy(m_Buffer, Data + RemainSize, sizeof(T) * (Size - RemainSize));
            }
         }
         //버퍼가 한바퀴 돌아 있는 경우
         else
            memcpy(m_Buffer + m_PosTail, Data, sizeof(T) * Size);
      }
      //Tail의 위치가 버퍼의 크기를 넘은 경우 원형큐의 앞으로 다시 이동
      m_PosTail = (m_PosTail + Size) % m_BufferSize;
      //입력 된 데이터의 크기를 DataSize에 저장
      m_DataSize += Size;

      return TRUE;
   };
   //버퍼 데이터 출력
   bool      Dequeue(T* Dest, DWORD Size) 
   {
      //출력해야할 데이터를 Dest에 저장
      if (Peek(Dest, Size) == FALSE)
         return false;

      //출력한 만큼 Head 위치 증가
      m_PosHead += Size;
      //버퍼 사이즈를 넘은 경우 원형큐의 원리로 다시 앞쪽으로 배치
      m_PosHead %= m_BufferSize;
      //출력 된 데이터의 크기를 DataSizoe에 저장
      m_DataSize -= Size;

      return TRUE;
   };

   bool      Peek(T* Dest, DWORD Size) 
   {
      //출력해야 하는 데이터의 크기가 저장된 데이터 크기보다 작으면 false 반환
      if (m_DataSize < Size)
         return FALSE;
      //출력할 값을 Dest에 저장
      if (Dest != 0)
      {
         //Head가 Tail보다 작으면 Dest에 바로 값 복사
         if (m_PosHead < m_PosTail)
            memcpy(Dest, m_Buffer + m_PosHead, sizeof(T) * Size);
         //같거나 Head가 크면 원형큐의 원리로 tail이 뒤에 있는 경우
         else
         {
            //Head 와 원형 큐의 끝자락 까지 남은 크기
            DWORD CutDataSize = m_BufferSize - m_PosHead;

            //  값 복사 해야 될 크기보다 남아있는 크기가 넉넉할 경우 바로 복사
            if (CutDataSize >= Size)
            {
               memcpy(Dest, m_Buffer + m_PosHead, sizeof(T) * Size);
            }
            // 남아 있는 크기가 다시 원형큐의 원리로 앞쪽에 넘어가 있는 경우
            else
            {
               //  데이터가 tail과 head로 나뉘어 저장된 경우
               memcpy(Dest, m_Buffer + m_PosHead, sizeof(T) * CutDataSize);
               memcpy(Dest + CutDataSize, m_Buffer, sizeof(T) * (Size - CutDataSize));
            }
         }
      }

      return TRUE;
   };


   T*		  GetBuffer() { return m_Buffer; }
   T*         GetRead() { return (m_Buffer + m_PosHead); }               //현재까지 읽은 데이터 위치 반환
   T*         GetWrite() { return (m_Buffer + m_PosTail); }               //현재까지 받아들인 데이터 위치 반환
   DWORD      GetRemainBufSize() { return m_BufferSize - m_DataSize; }      //남아 있는 데이터 크기 반환
   DWORD      GetDataSize() { return m_DataSize; }                     //입력된 데이터 크기 반환
   DWORD      GetBufferSize() { return m_BufferSize; }                  //총 입력할 수 있는 데이터 크기 반환
   //읽을 수 있는 남은 데이터 공간 크기 확인 함수
   DWORD      GetReadableSize()      
   {
      //Head 와 Tail가 같은 경우
      if (m_PosHead == m_PosTail)
      {
         //데이터 크기가 0 보다 크면 현재까지 읽은 데이터 위치 반환 작거나같으면 0 반환
         //0보다 작을 경우 원형 큐가 꽉착 경우 이므로 저장 불가  0반환
         return (m_DataSize > 0 ? m_BufferSize - m_PosHead : 0);
      }
      //Head 가 작은 경우
      else if (m_PosHead < m_PosTail)
      {
         //저장 할 수 있는 크기 반환
         return (m_PosTail - m_PosHead);
      }
      //Head가 큰 경우
      return (m_BufferSize - m_PosHead);
   };
   //쓰기를 할 수 있는 남은 데이터 공간 크기 확인 함수
   DWORD      GetWritableSize()
   {
      //Head 와 Tail가 같은 경우
      if (m_PosHead == m_PosTail)
      {
        // 데이터 크기가 0 보다 크면 현재까지 읽은 데이터 위치 반환 작거나같으면 0 반환
            //0보다 작을 경우 원형 큐가 꽉착 경우 이므로 저장 불가  0반환
         return (m_DataSize > 0 ? 0 : m_BufferSize - m_PosTail);
      }
      //Head 가 작은 경우
      else if (m_PosHead < m_PosTail)
      {
         //출력 할 수 있는 크기 반환
         return m_BufferSize - m_PosTail;
      }
      //Head가 큰 경우
      return m_PosHead - m_PosTail;
   };
};