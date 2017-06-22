#ifndef _CNgeVector_h_
#define _CNgeVector_h_
#include "string.h"
#define SAFE_DELETE(p) {if(p!=0) { delete p;p=0;}}
#define SAFE_CHACK(p) {if(p==0) return false}
template <typename T>
class CNgeVector
{
public:
	CNgeVector(){m_Size=0;m_BufSize=0;m_Buffer=0;}
	~CNgeVector(){clear();}
	inline int size(){return m_Size;}
	void       push_back(T& data);
	T          PopBack();
	T          PopFront();
	T&         operator[](int index);
	bool       Delete(int index);
	void       reserve(int size);
	void       clear();
private:
	int   m_Size;
	int   m_BufSize;
	T*    m_Buffer;
};
template <typename T>
void CNgeVector<T>::reserve(int size)
{
	m_Size=0;
	m_BufSize=size;
    m_Buffer=new T[size];
}
template <typename T>
void CNgeVector<T>::push_back(T& data)
{
     if(!m_Size&&!m_BufSize)
		 m_Buffer=new T(data);
	 else
	 {
          if(!m_BufSize)
		  {
			  T* tmp=new T[m_Size+1];
			  tmp[m_Size]=data;
			  memcpy(tmp,m_Buffer,sizeof(T)*m_Size);
			  SAFE_DELETE(m_Buffer);
			  m_Buffer=tmp;
			 
		  }else
		  {
			  m_Buffer[m_Size]=data;
              m_BufSize--;
		  }
	 }
	  m_Size++;
};
template <class T>
T    CNgeVector<T>::PopBack()
{
    T  value=m_Buffer[m_Size-1];
	Delete(m_Size-1);
	return value;
}
template <class T>
T    CNgeVector<T>::PopFront()
{
	 T value=m_Buffer[0];
     Delete(0);
     return value;
}
template <class T>
T&   CNgeVector<T>::operator[](int index)
{
	 return m_Buffer[index];
}
template <class T>
bool  CNgeVector<T>::Delete(int index)
{
	if (index>=m_Size)
		return false;
	memset(m_Buffer+index,0,sizeof(T));
    if(index+1<m_Size)
       memmove(m_Buffer+index,m_Buffer+index+1,(m_Size-index-1)*sizeof(T));
	m_Size--;
	m_BufSize++;
	return true;
}
template <class T>
void   CNgeVector<T>::clear()
{
	SAFE_DELETE(m_Buffer);
	m_Size=0;
	m_BufSize=0;

}
#endif
