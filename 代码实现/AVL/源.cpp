#include<iostream>
#include<queue>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

int max(int a, int b)
{
	return a > b ? a : b;
}

class avl_node
{
private:
	int value;
	int height;
	avl_node* lchild;
	avl_node* rchild;
public:
	friend class avl_tree;
	avl_node(int v, avl_node* l, avl_node* r):value(v),height(0),lchild(l),rchild(r){}
};

class avl_tree
{
private:
	avl_node* root;//���ڵ�
	avl_node* llrotation(avl_node* k2);//����ת��������ת��ĸ��ڵ�
	avl_node* rrrotation(avl_node* k1);//�ҵ���ת
	avl_node* lrrotation(avl_node* k3);//����˫��
	avl_node* rlrotation(avl_node* k1);//����˫��
	avl_node* search(avl_node*x, int v);
	avl_node* insert(avl_node* &r, int v);//���벢���ظ��ڵ�
	avl_node* remove(avl_node*& r, avl_node* z);//ɾ�������ظ��ڵ�
public:
	avl_tree() { root = NULL; }
	~avl_tree() { root = NULL; }
	int height(avl_node* node);//��ȡ���ĸ߶�
	void insert(int v);
	void remove(int v);
	bool output();//˳��������ж��Ƿ�Ϊ��ȫ������
};

int avl_tree::height(avl_node* node)
{
	if (node==NULL)return 0;
	else return (*node).height;
}

avl_node* avl_tree::llrotation(avl_node* k2)
{
	avl_node* k1;
	k1 = k2->lchild;
	k2->lchild = k1->rchild;
	k1->rchild = k2;
	k2->height = max(height(k2->lchild), height(k2->rchild)) + 1;
	k1->height = max(height(k1->lchild), height(k2)) + 1;
	return k1;
}

avl_node* avl_tree::rrrotation(avl_node* k1)
{
	avl_node* k2;
	k2 = k1->rchild;
	k1->rchild = k2->lchild;
	k2->lchild = k1;
	k1->height = max(height(k1->lchild), height(k1->rchild)) + 1;
	k2->height = max(height(k2->rchild), height(k1)) + 1;
	return k2;
}

avl_node* avl_tree::lrrotation(avl_node* k3)
{
	k3->lchild = rrrotation(k3->lchild);
	return llrotation(k3);
}

avl_node* avl_tree::rlrotation(avl_node* k1)
{
	k1->rchild = llrotation(k1->rchild);
	return rrrotation(k1);
}

avl_node* avl_tree::insert(avl_node* &r, int v)
{
	if (r == NULL)//����
	{
		r = new avl_node(v, NULL, NULL);
	}
	else if (v < r->value)//����������
	{
		r->lchild = insert(r->lchild, v);
		if (height(r->lchild) - height(r->rchild) == 2)//avlʧȥƽ��
		{
			if (v < r->lchild->value)r = llrotation(r);
			else r = lrrotation(r);
		}
	}
	else if (v > r->value)//����������
	{
		r->rchild = insert(r->rchild, v);
		if (height(r->rchild) - height(r->lchild) == 2)//avlʧȥƽ��
		{
			if (v > r->rchild->value)r = rrrotation(r);
			else r = rlrotation(r);
		}
	}
	else cout << "����ʧ�ܣ������������ͬ���" << endl;
	r->height = max(height(r->lchild), height(r->rchild)) + 1;
	return r;
}

void avl_tree::insert(int v)
{
	insert(root, v);
}

bool avl_tree::output()
{
	bool mark = 1;
	bool judge = 1;
	queue<avl_node>q;//����һ������
	q.push(*root);
	bool blank = 0;//����Ƿ�Ҫ�����ǰ����ո�
	while (!q.empty())
	{
		if (blank)cout << ' ';
		else blank = 1;
		cout << q.front().value;
		//�ж϶���Ԫ���Ƿ�����Ů
		if (q.front().lchild)
		{
			q.push(*(q.front().lchild));
			if (!mark)judge = 0;
		}
		else mark = 0;
		if (q.front().rchild)
		{
			q.push(*(q.front().rchild));
			if (!mark)judge = 0;
		}
		else mark = 0;
		q.pop();
	}
	cout << endl;
	return judge;
}

avl_node* avl_tree::search(avl_node*x, int v)
{
	while (x != NULL && x->value != v)
	{
		if (v > x->value)x = x->rchild;
		else x = x->lchild;
	}
	return x;
}

avl_node* avl_tree::remove(avl_node*& r, avl_node* z)
{
	if (r == NULL || z == NULL)return NULL;
	if (z->value < r->value)
	{
		r->lchild = remove(r->lchild, z);
		if (height(r->rchild) - height(r->lchild) == 2)
		{
			if (height(r->rchild->lchild) > height(r->rchild->rchild))
				r = rlrotation(r);
			else
				r = rrrotation(r);
		}
	}
	else if (z->value > r->value)
	{
		r->rchild = remove(r->rchild, z);
		if (height(r->lchild) - height(r->rchild) == 2)
		{
			if (height(r->lchild->rchild) > height(r->lchild->lchild))
				r = lrrotation(r);
			else
				r = llrotation(r);
		}
	}
	else
	{
		if (r->lchild != NULL && r->rchild != NULL)
		{
			if (height(r->lchild) > height(r->rchild))
			{
				avl_node* max = r->lchild;
				while (max->rchild)max = max->rchild;
				r->value = max->value;
				r->lchild = remove(r->lchild, max);
			}
			else
			{
				avl_node* min = r->rchild;
				while (min->lchild)min = min->lchild;
				r->value = min->value;
				r->rchild = remove(r->rchild, min);
			}
		}
		else
		{
			avl_node* tmp = r;
			r = (r->lchild != NULL) ? r->lchild : r->rchild;
			delete tmp;
		}
	}
	return r;
}

void avl_tree::remove(int key)
{
	avl_node* z;
	if ((z = search(root, key)) != NULL)
		root = remove(root, z);
}

void way1(avl_tree& tree, int n, ofstream& time_file)//������˳�����N������������ͬ˳��ɾ��
{
	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = 1; i <= n; i++)tree.insert(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "˳�����" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//��ʱ��ʼ
	for (int i = 1; i <= n; i++)tree.remove(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "˳��ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way2(avl_tree& tree, int n, ofstream& time_file)//������˳�����N�����������෴˳��ɾ��
{
	for (int i = 1; i <= n; i++)tree.insert(i);

	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = n; i > 0; i--)tree.remove(i);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "����ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way3(avl_tree& tree, int n, ofstream& time_file)//�����˳�����N�������������˳��ɾ��
{
	//�õ��������˳������
	int* a = new int[n];
	for (int i = 0; i < n; i++)a[i] = i + 1;
	random_shuffle(a, a + n);
	//�õ����ɾ��˳������
	int* b = new int[n];
	for (int i = 0; i < n; i++)b[i] = i + 1;
	random_shuffle(b, b + n);

	clock_t startTime, endTime;
	startTime = clock();//��ʱ��ʼ
	for (int i = 0; i < n; i++)tree.insert(a[i]);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "�������" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//��ʱ��ʼ
	for (int i = 0; i < n; i++)tree.remove(b[i]);
	endTime = clock();//��ʱ����

	if (time_file.is_open())
	{
		time_file << "���ɾ��" << n << "����������ʱ�䣺" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

int main()
{
	avl_tree tree;
	ofstream time_file("C:\\Users\\zxy\\Desktop\\avl_tree.txt", ios::app);
	for (int n = 100; n <= 1000000; n *= 10)
	{
		way1(tree, n, time_file);
		way2(tree, n, time_file);
		way3(tree, n, time_file);
	}
	time_file.close();
	return 0;
}
