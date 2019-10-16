//FIFO��first in first out
//���ζ���
#include<iostream>
using namespace std;
template<class T>
class queqe
{
public:
	queqe(int capacity);
	~queqe();
	void clear();							//��ն���
	bool empty() const;				//�пն���
	bool full()const;					//��������
	int length() const;					//���г���
	bool enter(T element);		//��Ԫ����ӣ���βָ���ƶ���
	bool dequeqe();						//��Ԫ�س��ӣ���ͷָ���ƶ���
	void queqe_traverse();			//��������
private:
	T* p_queqe;							//����ָ��
	int len;									//����Ԫ�ظ���
	int capacity_queqe;				//������������
	int head;								//	ͷָ�루�����±꣩
	int tail;									//βָ�루�����±꣩
};

template<class T>
queqe<T>::queqe(int capacity)
{
	p_queqe = new T[capacity];
	capacity_queqe = capacity;
	len = 0;
	head = 0;
	tail = 0;
}

template<class T>
queqe<T>::~queqe()
{
	delete []p_queqe;
	p_queqe = NULL;
}

template<class T>
void queqe<T>::clear()
{
	head = 0;
	tail = 0;
	len = 0;
}

template<class T>
bool queqe<T>::empty() const
{
	return len==0 ? true : false;
}

template<class T>
bool queqe<T>::full()const
{
	return len == capacity_queqe ? true : false;
}

template<class T>
int queqe<T>::length()const
{
	return len;
}

template<class T>
bool queqe<T>::enter(T element)
{
	if (full())return false;
	else
	{
		p_queqe[tail] = element;
		tail++;
		tail %= capacity_queqe;
		len++;
		return true;
	}
}

template<class T>
bool queqe<T>::dequeqe()
{
	if (empty())return false;
	head++;
	head %= capacity_queqe;
	len--;
	return true;
}

template<class T>
void queqe<T>::queqe_traverse()
{
	cout << endl;
	for (int i = head,num=0; num<len; i = (i+1)%capacity_queqe)
	{
		cout << p_queqe[i]<<' ';
		num++;
	}
	cout << endl;
}