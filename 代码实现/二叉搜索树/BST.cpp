#include<iostream>
#include <ctime>
#include <algorithm>
#include <fstream>
using namespace std;

class bst_node
{
public:
	int value;
	bst_node* lchild;
	bst_node* rchild;
	bst_node* parent;
	bst_node():value(0),lchild(NULL),rchild(NULL),parent(NULL){}
	bst_node(int key):value(key),lchild(NULL),rchild(NULL),parent(NULL){}
};

class bs_tree
{
private:
	bst_node* root;
	void insert(bst_node* &root, bst_node* node);
	bst_node* search(bst_node*root,int key);
	void remove(bst_node*& root, bst_node* node);
public:
	bs_tree():root(NULL){}
	void insert(int key);
	void remove(int key);
};

void swap(bst_node* a, bst_node* b)
{
	int tmp = a->value;
	a->value = b->value;
	b->value = tmp;
}

void bs_tree::insert(bst_node* &root, bst_node* node)
{
	bst_node* x = root;
	bst_node* parent = NULL;
	while (x)
	{
		parent = x;
		if (node->value < x->value) x = x->lchild; 
		else if (node->value > x->value)x = x->rchild;
		else return;
	}
	if (parent)
	{
		if (node->value < parent->value) {
			parent->lchild = node; node->parent = parent;
		}
		else {
			parent->rchild = node; node->parent = parent;
		}
		return;
	}
	root = node;
}

void bs_tree::insert(int key)
{
	bst_node* node = new bst_node(key);
	insert(root, node);
}

bst_node* bs_tree::search(bst_node*root,int key)
{
	bst_node* node = root;
	while (node)
	{
		if (key < node->value)node = node->lchild;
		else if (key > node->value)node = node->rchild;
		else return node;
	}
	return NULL;
}

void bs_tree::remove(bst_node*& root, bst_node* node)
{
	//node无孩子，直接删除
	if (node->lchild == NULL && node->rchild == NULL)
	{
		//node是根节点
		if (node == root)
		{
			root = NULL;
			delete node;
		}
		else
		{
			if (node->parent->lchild == node)
			{
				node->parent->lchild = NULL; 
				delete node;
			}
			else
			{
				node->parent->rchild = NULL; 
				delete node;
			}
		}
	}

	//node只有一个孩子
	else if (node->lchild == NULL)
	{
		swap(node, node->rchild);
		delete node->rchild;
		node->rchild = NULL;
	}
	else if (node->rchild == NULL)
	{
		swap(node, node->lchild);
		delete node->lchild;
		node->lchild = NULL;
	}

	//node有两个孩子
	else
	{
		bst_node* replace = node->rchild;
		while (replace->lchild)replace = replace->lchild;

		swap(node, replace);

		if (node->rchild == replace)
		{
			node->rchild = replace->rchild;
			if(replace->rchild)replace->rchild->parent = node;
		}
		else
		{
			replace->parent->lchild = replace->rchild;
			if(replace->rchild)replace->rchild->parent = replace->parent;
		}
		delete replace;
	}
}

void bs_tree::remove(int key)
{
	bst_node* node = search(root, key);
	if (node)remove(root, node);
}

void way1(bs_tree& tree, int n, ofstream& time_file)//按递增顺序插入N个整数，按相同顺序删除
{
	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = 1; i <= n; i++)tree.insert(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "顺序插入" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//计时开始
	for (int i = 1; i <= n; i++)tree.remove(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "顺序删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way2(bs_tree& tree, int n, ofstream& time_file)//按递增顺序插入N个整数，按相反顺序删除
{
	for (int i = 1; i <= n; i++)tree.insert(i);

	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = n; i > 0; i--)tree.remove(i);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "逆序删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

void way3(bs_tree& tree, int n, ofstream& time_file)//按随机顺序插入N个整数，按随机顺序删除
{
	//得到随机插入顺序数组
	int* a = new int[n];
	for (int i = 0; i < n; i++)a[i] = i + 1;
	random_shuffle(a, a + n);
	//得到随机删除顺序数组
	int* b = new int[n];
	for (int i = 0; i < n; i++)b[i] = i + 1;
	random_shuffle(b, b + n);

	clock_t startTime, endTime;
	startTime = clock();//计时开始
	for (int i = 0; i < n; i++)tree.insert(a[i]);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "随机插入" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}

	startTime = clock();//计时开始
	for (int i = 0; i < n; i++)tree.remove(b[i]);
	endTime = clock();//计时结束

	if (time_file.is_open())
	{
		time_file << "随机删除" << n << "个整数所用时间：" << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	}
}

int main()
{
	bs_tree tree;
	ofstream time_file("C:\\Users\\zxy\\Desktop\\bs_tree.txt", ios::app);
	for (int n = 100; n <= 1000000; n *= 10)
	{
		way1(tree, n, time_file);
		way2(tree, n, time_file);
		way3(tree, n, time_file);
	}
	time_file.close();
	return 0;
}