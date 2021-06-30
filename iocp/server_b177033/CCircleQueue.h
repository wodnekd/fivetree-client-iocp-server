#pragma once

template< class T >
class CCircleQueue
{
private:
   T*         m_Buffer;               //�����͸� ������ ����
   DWORD      m_BufferSize;            //�� ������ ũ��
   DWORD      m_DataSize;               //�������� ũ��
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
   // ������, ���, ���� �ʱ�ȭ
   void Clear() 
   {
      m_DataSize = 0;
      m_PosHead  = 0;
      m_PosTail  = 0;
   };
   //���� ����
   void Destroy()
   {
      if (m_Buffer != 0) 
      {
         delete[] m_Buffer;
      }
      m_Buffer = 0;
      Clear();
   };
   //���� ũ�� ����
   BOOL Create(DWORD BufferSize) 
   {
      if (m_Buffer != 0)   return FALSE;

      m_Buffer      = new T[BufferSize];         //�Էµ�bufferSize ��ŭ ���� ����
      m_BufferSize   = BufferSize;               //�Էµȹ��� ũ�� ����
      memset(m_Buffer, 0, sizeof(m_Buffer));         //���۸� 0���� �ʱ�ȭ
      Clear();                              //���� ��뿡 �ʿ��� ���� ���� �ʱ�ȭ
      return TRUE;
   };
   //���ۿ� ������ �Է�
   bool Enqueue(T* Data, DWORD Size) 
   {
      //���ۿ� ������ ������ ������ ���� ���� �Ұ�
      if (GetRemainBufSize() < Size) return FALSE;
      //Data ���� ���ۿ� ����
      if (Data != 0)
      {
         //�ʱ� ����, H
         if (m_PosHead <= m_PosTail)
         {
            //���� ���� ũ�� RemainSize�� ����
            DWORD      RemainSize = m_BufferSize - m_PosTail;

            //  ���� ���� ũ�Ⱑ ���� ������ ũ�⺸�� ũ�ٸ� �ѹ��� �����Ѵ�.
            if (RemainSize >= Size)
            {
               //m_Buffer ó�� ��ġ + m_PosTail(�ԷµǾ��� ��ġ)
               memcpy
               (
                  m_Buffer + m_PosTail,               //���縦 ���ϴ� �ּ� 
                  Data,                           //���縦 �ϴ� �ּ�
                  sizeof(T) * Size                  //������ ����Ʈ ����
               );
            }
            else
            {
               //  ���ۿ� ������ ���ٸ� �����͸� �ΰ��� ������
               //  ������ ��/�ڿ� �����Ѵ�.
               memcpy(m_Buffer + m_PosTail, Data, sizeof(T) * RemainSize);
               memcpy(m_Buffer, Data + RemainSize, sizeof(T) * (Size - RemainSize));
            }
         }
         //���۰� �ѹ��� ���� �ִ� ���
         else
            memcpy(m_Buffer + m_PosTail, Data, sizeof(T) * Size);
      }
      //Tail�� ��ġ�� ������ ũ�⸦ ���� ��� ����ť�� ������ �ٽ� �̵�
      m_PosTail = (m_PosTail + Size) % m_BufferSize;
      //�Է� �� �������� ũ�⸦ DataSize�� ����
      m_DataSize += Size;

      return TRUE;
   };
   //���� ������ ���
   bool      Dequeue(T* Dest, DWORD Size) 
   {
      //����ؾ��� �����͸� Dest�� ����
      if (Peek(Dest, Size) == FALSE)
         return false;

      //����� ��ŭ Head ��ġ ����
      m_PosHead += Size;
      //���� ����� ���� ��� ����ť�� ������ �ٽ� �������� ��ġ
      m_PosHead %= m_BufferSize;
      //��� �� �������� ũ�⸦ DataSizoe�� ����
      m_DataSize -= Size;

      return TRUE;
   };

   bool      Peek(T* Dest, DWORD Size) 
   {
      //����ؾ� �ϴ� �������� ũ�Ⱑ ����� ������ ũ�⺸�� ������ false ��ȯ
      if (m_DataSize < Size)
         return FALSE;
      //����� ���� Dest�� ����
      if (Dest != 0)
      {
         //Head�� Tail���� ������ Dest�� �ٷ� �� ����
         if (m_PosHead < m_PosTail)
            memcpy(Dest, m_Buffer + m_PosHead, sizeof(T) * Size);
         //���ų� Head�� ũ�� ����ť�� ������ tail�� �ڿ� �ִ� ���
         else
         {
            //Head �� ���� ť�� ���ڶ� ���� ���� ũ��
            DWORD CutDataSize = m_BufferSize - m_PosHead;

            //  �� ���� �ؾ� �� ũ�⺸�� �����ִ� ũ�Ⱑ �˳��� ��� �ٷ� ����
            if (CutDataSize >= Size)
            {
               memcpy(Dest, m_Buffer + m_PosHead, sizeof(T) * Size);
            }
            // ���� �ִ� ũ�Ⱑ �ٽ� ����ť�� ������ ���ʿ� �Ѿ �ִ� ���
            else
            {
               //  �����Ͱ� tail�� head�� ������ ����� ���
               memcpy(Dest, m_Buffer + m_PosHead, sizeof(T) * CutDataSize);
               memcpy(Dest + CutDataSize, m_Buffer, sizeof(T) * (Size - CutDataSize));
            }
         }
      }

      return TRUE;
   };


   T*		  GetBuffer() { return m_Buffer; }
   T*         GetRead() { return (m_Buffer + m_PosHead); }               //������� ���� ������ ��ġ ��ȯ
   T*         GetWrite() { return (m_Buffer + m_PosTail); }               //������� �޾Ƶ��� ������ ��ġ ��ȯ
   DWORD      GetRemainBufSize() { return m_BufferSize - m_DataSize; }      //���� �ִ� ������ ũ�� ��ȯ
   DWORD      GetDataSize() { return m_DataSize; }                     //�Էµ� ������ ũ�� ��ȯ
   DWORD      GetBufferSize() { return m_BufferSize; }                  //�� �Է��� �� �ִ� ������ ũ�� ��ȯ
   //���� �� �ִ� ���� ������ ���� ũ�� Ȯ�� �Լ�
   DWORD      GetReadableSize()      
   {
      //Head �� Tail�� ���� ���
      if (m_PosHead == m_PosTail)
      {
         //������ ũ�Ⱑ 0 ���� ũ�� ������� ���� ������ ��ġ ��ȯ �۰ų������� 0 ��ȯ
         //0���� ���� ��� ���� ť�� ���� ��� �̹Ƿ� ���� �Ұ�  0��ȯ
         return (m_DataSize > 0 ? m_BufferSize - m_PosHead : 0);
      }
      //Head �� ���� ���
      else if (m_PosHead < m_PosTail)
      {
         //���� �� �� �ִ� ũ�� ��ȯ
         return (m_PosTail - m_PosHead);
      }
      //Head�� ū ���
      return (m_BufferSize - m_PosHead);
   };
   //���⸦ �� �� �ִ� ���� ������ ���� ũ�� Ȯ�� �Լ�
   DWORD      GetWritableSize()
   {
      //Head �� Tail�� ���� ���
      if (m_PosHead == m_PosTail)
      {
        // ������ ũ�Ⱑ 0 ���� ũ�� ������� ���� ������ ��ġ ��ȯ �۰ų������� 0 ��ȯ
            //0���� ���� ��� ���� ť�� ���� ��� �̹Ƿ� ���� �Ұ�  0��ȯ
         return (m_DataSize > 0 ? 0 : m_BufferSize - m_PosTail);
      }
      //Head �� ���� ���
      else if (m_PosHead < m_PosTail)
      {
         //��� �� �� �ִ� ũ�� ��ȯ
         return m_BufferSize - m_PosTail;
      }
      //Head�� ū ���
      return m_PosHead - m_PosTail;
   };
};