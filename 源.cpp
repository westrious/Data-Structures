//FIFO：first in first out
//环形队列
#include<iostream>
using namespace std;
template<class T>
class queqe
{
public:
	queqe(int capacity);
	~queqe();
	void clear();							//清空队列
	bool empty() const;				//判空队列
	bool full()const;					//判满队列
	int length() const;					//队列长度
	bool enter(T element);		//新元素入队（队尾指针移动）
	bool dequeqe();						//首元素出队（队头指针移动）
	void queqe_traverse();			//遍历队列
private:
	T* p_queqe;							//队列指针
	int len;									//队列元素个数
	int capacity_queqe;				//队列数组容量
	int head;								//	头指针（数组下标）
	int tail;									//尾指针（数组下标）
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